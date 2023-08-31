// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"
#include "gpio.h"
#include "uart.h"
#include "printf.h"
/* Mailbox Data Buffer (each element is 32-bit)*/
/*
 * The keyword attribute allows you to specify special attributes
 *
 * The aligned(N) attribute aligns the current data item on an address
 * which is a multiple of N, by inserting padding bytes before the data item
 *
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
 *
 *
 * We must ensure that our our buffer is located at a 16 byte aligned address,
 * so only the high 28 bits contain the address
 * (last 4 bits is ZERO due to 16 byte alignment)
 *
 */
volatile unsigned int __attribute__((aligned(16))) mBuf[36];
/**
 * Read from the mailbox
 */

void board_revision() {
    mBuf[0] = 7 * 4;
    mBuf[1] = MBOX_REQUEST;
    mBuf[2] = 0x00010002;
    mBuf[3] = 4;
    mBuf[5] = 0;
    mBuf[6] = 0;
    mBuf[7] = MBOX_TAG_LAST;

    if(mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("\nBoard revision: ");
        uart_hex(mBuf[5]);
        uart_puts("\n");
    } else {
        uart_puts("INVALID\n");
    }
}

void board_mac_address() {
    mBuf[0] = 7 * 4;
    mBuf[1] = MBOX_REQUEST;
    mBuf[2] = MBOX_TAG_GETMACADD;
    mBuf[3] = 6;
    mBuf[4] = 0;
    mBuf[5] = 0;
    mBuf[6] = MBOX_TAG_LAST;

    if(mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("\nBoard MAC address: ");
        uart_hex(mBuf[5]);
    } else {
        uart_puts("INVALID\n");
    }
}

void mbox_buffer_setup(unsigned int buffer_addr, 
unsigned int tag_identifier, 
unsigned int **res_data, 
unsigned int res_length, 
unsigned int req_length, ...) {

    // Set up the header of the message
    mBuf[0] = 17*4;
    mBuf[1] = 0;                  // Request/response code
    mBuf[2] = tag_identifier;     // Tag identifier

    va_list args;
    va_start(args, req_length);

    for (unsigned int i = 0; i < req_length >>2; i++) {
        mBuf[3 + i] = va_arg(args, unsigned int); // Copy request values
    }
    va_end(args);

    //set the res_data pointer to point to the response data
    *res_data = mBuf + 5;

    switch(tag_identifier) {
        case MBOX_TAG_GETMODEL:
            // mailbox data buffer: Read ARM frequency
            mBuf[3] = 4;  // Value buffer size in bytes (max of request and response lengths)
            mBuf[4] = 0;			   // REQUEST CODE = 0
            mBuf[5] = 0;                // clear output buffer (response data are mBuf[5])
            mBuf[6] = MBOX_TAG_LAST;
            if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
                uart_puts("Board Model:  ");
                uart_hex(mBuf[5]);
                uart_puts("\n\n");

            }
            else {
                uart_puts("INVALID!\n");
            }
            break;
        case MBOX_TAG_SETPHYWH:
            mBuf[3] = 8;     // Value buffer size in bytes
            mBuf[4] = 0;     // Length of request values in bytes
            mBuf[5] = va_arg(args, unsigned int);    //width
            mBuf[6] = va_arg(args, unsigned int);    //height
            mBuf[7] = MBOX_TAG_LAST;
            break;
        case MBOX_TAG_GETSERIAL:
            mBuf[3] = 8;
            mBuf[4] = 0;
            mBuf[5] = 0;
            mBuf[6] = MBOX_TAG_LAST;

            if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
                uart_puts("Board Serial:  ");
                uart_hex(mBuf[5]);
                uart_puts("\n\n");
            } 
            else {
                uart_puts("INVALID!\n");
            }
            break;
        case MBOX_TAG_GETMACADD: 
            mBuf[3] = 6;
            mBuf[4] = 0;
            mBuf[5] = 0;
            mBuf[6] = MBOX_TAG_LAST;

            if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
                uart_puts("Board MAC Address: ");
                uart_hex(mBuf[5]);
                uart_puts("\n\n");
            } 
            else {
                uart_puts("INVALID!\n");
            }
            break;
        case MBOX_TAG_SETCLKRATE:
            mBuf[2] = 0x00030002;
            mBuf[3] = 8;
            mBuf[4] = 0;
            mBuf[5] = 3;
            mBuf[6] = 0;
            mBuf[7] = MBOX_TAG_LAST;

            if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
                uart_puts("ARM frequency clock rate: ");
                uart_dec(mBuf[6]);
                uart_puts("\n\n");
            } 
            else {
                uart_puts("INVALID!\n");
            }
            break;
        default:
            break;
    }
}

uint32_t mailbox_read(unsigned char channel) {
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = MBOX0_READ;
    } while ((res & 0xF) != channel);
    return res;
}
/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
        asm volatile("nop");
    } while (MBOX1_STATUS & MBOX_FULL);
    // send the message
    MBOX1_WRITE = msg;
}
/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
    // Check Buffer Address
    uart_puts("Buffer Address: ");
    uart_hex(buffer_addr);
    uart_sendc('\n');
    // Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);
    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            uart_puts("Got successful response \n");
        return (mBuf[1] == MBOX_RESPONSE);
    }
    return 0;
}