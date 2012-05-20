#include "../../include/defs.h"
#include "../../include/structs.h"
#include "../../include/keyboard.h"

struct key_t * key;
extern struct tty_t ttys[];
extern Task_t * current_task;
extern Task_t * foreground_task;
int i=0;

char e_lowercase[256] =
		{ 0, 0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
				'\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
				'[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
				';', '\'', '\\', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
				',', '.', '/', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64,
				65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
				81, 82, 83, 84, 85, '<' };

char e_uppercase[256] =
		{ 0, 0x1b, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
				'\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
				'{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
				':', '"', 0xb3, 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
				'>', '?', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65,
				66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
				82, 83, 84, 85, '>' };

char e_capslowercase[256] = { 0, 0x1b, '1', '2', '3', '4', '5', '6', '7', '8',
		'9', '0', '-', '=', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
		'O', 'P', '[', ']', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
		'L', ';', '\'', '\\', 0, 0x60, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',',
		'.', '-', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
		68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
		'<' };

char e_capsuppercase[256] = { 0, 0x1b, '!', '@', '#', '$', '%', '^', '&', '*',
		'(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
		'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
		'L', ':', '"', 0xb3, 0, '~', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
		'>', '?', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
		68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
		'>' };

char s_lowercase[256] = { 0, 0xb3, '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'0', '\'', 0xa8, '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
		'o', 'p', '´', '+', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
		'l', A_ENIE_MIN, '{', 0xb3, 0, '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
		',', '.', '-', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65, 66,
		67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
		85, '<' };

char s_uppercase[256] = { 0, 0xa7, '!', '"', '#', '$', '%', '&', '/', '(', ')',
		'=', '?', 0xad, '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
		'O', 'P', '¨', '*', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',
		'L', A_ENIE_MAY, '[', 0xa7, 42, ']', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		';', ':', '_', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65, 66,
		67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
		85, '>' };

char s_capslowercase[256] = { 0, 0xb3, '1', '2', '3', '4', '5', '6', '7', '8',
		'9', '0', '\'', 0xa8, '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
		'I', 'O', 'P', '´', '+', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J',
		'K', 'L', A_ENIE_MAY, '{', 0xb3, 0, '}', 'Z', 'X', 'C', 'V', 'B', 'N',
		'M', ',', '.', '-', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65,
		66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
		84, 85, '<' };

char s_capsuppercase[256] = { 0, 0xa7, '!', '"', '#', '$', '%', '&', '/', '(',
		')', '=', '?', 0xad, '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u',
		'i', 'o', 'p', '¨', '*', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
		'k', 'l', A_ENIE_MIN, '[', 0xa7, 42, ']', 'z', 'x', 'c', 'v', 'b', 'n',
		'm', ';', ':', '_', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65,
		66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
		84, 85, '>' };

unsigned char insertKey(unsigned char c) {
	keyboard_t * keyboard = getKeyboard(foreground_task);

	if(foreground_task->state == TaskSuspended)
	{
		unsuspend_task(foreground_task);
	}
	if (!bufferIsFull()) {
		kprintf("\n%c\n", c);
		keyboard->buffer[keyboard->tail] = c;
		if (++(keyboard->tail) == K_BUFFER_SIZE) {
			keyboard->tail = 0;
			return c;
		}
	}
	return 0;
}

unsigned char getKey() {

	keyboard_t * keyboard = getKeyboard(current_task);

	suspend_task(current_task);
	yield();
	int head = keyboard->head;
	if (!isEmptyBuffer()) {
		keyboard->buffer[keyboard->head];
		if (++(keyboard->head) == K_BUFFER_SIZE) {
			keyboard->head = 0;
		}

		return keyboard->buffer[head];
	}

	return EMPTY_CHAR;

}

int bufferIsFull() {

	keyboard_t * keyboard = getKeyboard(foreground_task);
	kprintf(" --%d--. ", &keyboard->buffer);
	if ((keyboard->tail + 1) % K_BUFFER_SIZE
			== keyboard->head) {
		return TRUE;
	}
	return FALSE;
}

int isEmptyBuffer() {

	keyboard_t * keyboard = getKeyboard(foreground_task);

	if (keyboard->tail == keyboard->head) {
		return TRUE;
	}
	return FALSE;
}

struct key_t * parseKey(unsigned char c) {

	if(current_task == NULL || foreground_task == NULL)
		return;

	keyboard_t * keyboard = getKeyboard(foreground_task);

	if (keyboard->escaped_key) {
		c += 256;
		keyboard->escaped_key = 0;
	}
	switch (c) {
	case NUMLOCK:
		keyboard->num_state = !keyboard->num_state;
		key->keyType = HIDDEN_KEY;
		updateLeds();
		break;
	case SCROLLLOCK:
		keyboard->scroll_state = !keyboard->scroll_state;
		key->keyType = HIDDEN_KEY;
		updateLeds();
		break;
	case LEFT_SHIFT_PRESSED:
	case RIGHT_SHIFT_PRESSED:
		keyboard->shift_state = TRUE;
		if (keyboard->ctrl_state == TRUE)
			changeLang();
		key->keyType = HIDDEN_KEY;
		break;
	case RIGHT_SHIFT_RELEASED:
	case LEFT_SHIFT_RELEASED:
		keyboard->shift_state = FALSE;
		key->keyType = HIDDEN_KEY;
		break;
	case CAPSLOCK:
		keyboard->caps_state = !keyboard->caps_state;
		updateLeds();
		key->keyType = HIDDEN_KEY;
		break;
	case ESCAPED_KEY:
		keyboard->escaped_key = TRUE;
		key->keyType = HIDDEN_KEY;
		break;
	case ALT_PRESSED:
		keyboard->alt_state = TRUE;
		break;
	case ALT_RELEASED:
		keyboard->alt_state = FALSE;
		break;
	case CONTROL_PRESSED:
		keyboard->ctrl_state = TRUE;
		if (keyboard->shift_state == TRUE)
			changeLang();
		break;
	case CONTROL_RELEASED:
		keyboard->ctrl_state = FALSE;
		break;
	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
	case PAGE_UP:
	case PAGE_DOWN:
		key->keyType = FUNCTION_KEY;
		break;
	case F1:
	case F2:
	case F3:
	case F4:
		key->keyType = FUNCTION_KEY;
		changeTTY(c - 0x3b);
		break;
	case DEAD_KEY:
		if (keyboard->lang == SPANISH) {
			keyboard->dead_key = TRUE;
			break;
		}
	default:
		if (c & 0x80) {
			key->keyType = HIDDEN_KEY;
		} else {

				key->keyType = ALPHANUM_KEY;
				if (keyboard->caps_state) {
					if (keyboard->lang == ENGLISH) {
						key->ascii = (keyboard->shift_state ? e_capsuppercase
								: e_capslowercase)[c];
					} else if (keyboard->lang == SPANISH) {
						if (keyboard->dead_key && keyboard->shift_state
								&& isVowel(c)) {
							key->ascii = accVowel(c);
						} else
							key->ascii
									= (keyboard->shift_state ? s_capsuppercase
											: s_capslowercase)[c];
					}
				} else {
					if (keyboard->lang == ENGLISH) {
						key->ascii = (keyboard->shift_state ? e_uppercase
								: e_lowercase)[c];
					} else if (keyboard->lang == SPANISH) {
						if (keyboard->dead_key && !keyboard->shift_state
								&& isVowel(c)) {
							key->ascii = accVowel(c);
						} else
							key->ascii = (keyboard->shift_state ? s_uppercase
									: s_lowercase)[c];
					}
				}
				keyboard->dead_key = FALSE;
		}
		break;

	}

	return key;
}

int isPrintable(unsigned char c) {
	if ((c > 1 && c < 29) || (c > 29 && c < 42) || (c > 42 && c < 55) || c
			== 86 || c == 57) {
		return 1;
	}
	return 0;
}
void changeLang() {

	keyboard_t * keyboard = getKeyboard(current_task);
	ttyScreen_t * screen = getScreen(current_task);

	if (keyboard->lang == ENGLISH)
		keyboard->lang = SPANISH;
	else
		keyboard->lang = ENGLISH;
	print_header();
	move_cursor(screen->wpos / 2);
}

int isVowel(char c) {
	if (c == 0x12 || c == 0x1e || c == 0x16 || c == 0x17 || c == 0x18)
		return TRUE;
	return FALSE;
}

char accVowel(char c) {
	char ch;
	switch (c) {
	case 0x12:
		ch = 'é';
		break;
	case 0x1e:
		ch = 'á';
		break;
	case 0x16:
		ch = 'ú';
		break;
	case 0x17:
		ch = 'í';
		break;
	case 0x18:
		ch = 'ó';
		break;
	}
	return ch;
}

void viewBuffer() {
	putc(getc());
}

void updateLeds() {

	keyboard_t * keyboard = getKeyboard(current_task);

	unsigned char leds = keyboard->scroll_state
			| (keyboard->num_state * 2)
			| (keyboard->caps_state * 4);
	_out(0x60, 0xed);
	while (_in(0x60) != 0xfa)
		;
	_out(0x60, leds);
}
