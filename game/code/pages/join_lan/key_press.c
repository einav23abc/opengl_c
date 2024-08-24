#include "page.h"
#include "../../alerts.h"

char ip_input_string[16] = {[0 ... 14] = ' ', [15] = '\0'};
int8_t ip_input_current_char = 0;
int8_t ip_input_current_byte = 0;
int8_t ip_input_byte_current_number[4] = {0};

void key_press_join_lan() {
    SDL_Scancode scancode = event.key.keysym.scancode;
    switch (scancode) {
        case SDL_SCANCODE_1:
        case SDL_SCANCODE_2:
        case SDL_SCANCODE_3:
        case SDL_SCANCODE_4:
        case SDL_SCANCODE_5:
        case SDL_SCANCODE_6:
        case SDL_SCANCODE_7:
        case SDL_SCANCODE_8:
        case SDL_SCANCODE_9:
        case SDL_SCANCODE_0:
        {
            int32_t number = scancode - SDL_SCANCODE_1 + 1;
            if (scancode == SDL_SCANCODE_0) number -= 10;

            if (ip_input_current_byte >= 4) break;
            if (ip_input_current_byte == 3 && ip_input_byte_current_number[ip_input_current_byte] >= 3) break;

            if (ip_input_byte_current_number[ip_input_current_byte] >= 3) {
                ip_input_string[ip_input_current_char] = '.';
                ip_input_current_char += 1;
                ip_input_current_byte += 1;
                ip_input_byte_current_number[ip_input_current_byte] = 0;
            }

            ip_input_string[ip_input_current_char] = '0' + number;
            ip_input_current_char += 1;
            ip_input_byte_current_number[ip_input_current_byte] += 1;

            break;
        }
        case SDL_SCANCODE_PERIOD:
        case SDL_SCANCODE_SPACE:
        {
            if (ip_input_current_byte >= 3) break;

            if (ip_input_byte_current_number[ip_input_current_byte] == 0) {
                ip_input_string[ip_input_current_char] = '0';
                ip_input_current_char += 1;
                ip_input_byte_current_number[ip_input_current_byte] = 1;
            }
            ip_input_string[ip_input_current_char] = '.';
            ip_input_current_char += 1;
            ip_input_current_byte += 1;
            ip_input_byte_current_number[ip_input_current_byte] = 0;

            break;
        }
        case SDL_SCANCODE_DELETE:
        case SDL_SCANCODE_BACKSPACE:
        {
            if (ip_input_current_byte == 0 && ip_input_byte_current_number[ip_input_current_byte] == 0) break;

            ip_input_current_char -= 1;
            ip_input_string[ip_input_current_char] = ' ';

            if (ip_input_byte_current_number[ip_input_current_byte] == 0) {
                ip_input_current_byte -= 1;
            }else {
                ip_input_byte_current_number[ip_input_current_byte] -= 1;
            }

            break;
        }
        case SDL_SCANCODE_RETURN: {
            attemp_lan_join();
            break;
        }
        default:
            add_error_alert_at_cursor("Enter a IP - numbers and periods only");
            break;
    }
}