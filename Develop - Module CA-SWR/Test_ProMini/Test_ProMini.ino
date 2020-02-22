/*Để nạp chương trình cho ProMini nhờ mạch nạp của Arduino UNO. Ta chuẩn bị một Arduino UNO bất kì. 
 * 1. Nạp một chương trình trắng cho Arduino UNO này, chương trình trắng chỉ bao gồm phần setup() và loop() mà không có các 
 * nội dung thao tác. (chọn đúng Board UNO và COM kết nối).
 * 2. Kết nối giữa Arduino UNO và ProMini như sau:
 *          UNO         ProMini
 *          5V            VCC
 *          GND           GND
 *          TX            TX
 *          RX            RX
 *          RTS           DTR
 * 3. Kết nối lại USB UART và chọn Board ProMini trong list board cùng với loại thích hợp và COM kết nối.
 * 4. Bắt đầu nạp code trực tiếp cho ProMini như bình thường.
 */
int PIN_1 = 9;
int PIN_2 = 10;
int PIN_3 = 11;
int PIN_4 = 12;
int PIN_5 = 13;
void setup() {
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PIN_3, OUTPUT);
    pinMode(PIN_4, OUTPUT);
    pinMode(PIN_5, OUTPUT);
}

void loop() {
    digitalWrite(PIN_1, HIGH);
    digitalWrite(PIN_2, HIGH);
    digitalWrite(PIN_3, HIGH);
    digitalWrite(PIN_4, HIGH);
    digitalWrite(PIN_5, HIGH);
    delay(500);

    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_2, LOW);
    digitalWrite(PIN_3, LOW);
    digitalWrite(PIN_4, LOW);
    digitalWrite(PIN_5, LOW); 

    delay(500);
}
