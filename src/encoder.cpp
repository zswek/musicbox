#include "encoder.h"

int Encoder::is_utf8_special_byte(unsigned char c)
{
    unsigned special_byte = 0X02; //binary 00000010    
    if (c >> 6 == special_byte) {
        return 1;
    } else {
        return 0;
    }
}

double Encoder::is_utf8_code(QByteArray str)
{
    unsigned one_byte = 0X00; //binary 00000000
    unsigned two_byte = 0X06; //binary 00000110
    unsigned three_byte = 0X0E; //binary 00001110  
    unsigned four_byte = 0X1E; //binary 00011110
    unsigned five_byte = 0X3E; //binary 00111110
    unsigned six_byte = 0X7E; //binary 01111110
    int utf8_yes = 0;
    int utf8_no = 0;
    unsigned char k = 0;
    unsigned char m = 0;
    unsigned char n = 0;
    unsigned char p = 0;
    unsigned char q = 0;
    unsigned char c = 0;
    for (unsigned int i=0; i<str.size();) {
        c = (unsigned char)str.at(i);
        if (c>>7 == one_byte) {
            i++;
            continue;
        } else if (c>>5 == two_byte) {
            k = (unsigned char)str.at(i+1);
            if ( is_utf8_special_byte(k) ) {
                utf8_yes++;
                i += 2;
                continue;
            }
        } else if (c>>4 == three_byte) {
            m = (unsigned char)str.at(i+1);
            n = (unsigned char)str.at(i+2);
            if ( is_utf8_special_byte(m)
                    && is_utf8_special_byte(n) ) {
                utf8_yes++;
                i += 3;
                continue;
            }
        } else if (c>>3 == four_byte) {
            k = (unsigned char)str.at(i+1);
            m = (unsigned char)str.at(i+2);
            n = (unsigned char)str.at(i+3);
            if ( is_utf8_special_byte(k)
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n) ) {
                utf8_yes++;
                i += 4;
                continue;
            }
        } else if (c>>2 == five_byte) {
            unsigned char k = (unsigned char)str.at(i+1);
            unsigned char m = (unsigned char)str.at(i+2);
            unsigned char n = (unsigned char)str.at(i+3);
            unsigned char p = (unsigned char)str.at(i+4);
            if ( is_utf8_special_byte(k) 
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n)
                    && is_utf8_special_byte(p) ) {
                utf8_yes++;
                i += 5;
                continue;
            }
        } else if (c>>1 == six_byte) {
            k = (unsigned char)str.at(i+1);
            m = (unsigned char)str.at(i+2);
            n = (unsigned char)str.at(i+3);
            p = (unsigned char)str.at(i+4);
            q = (unsigned char)str.at(i+5);
            if ( is_utf8_special_byte(k)
                    && is_utf8_special_byte(m)
                    && is_utf8_special_byte(n)
                    && is_utf8_special_byte(p)
                    && is_utf8_special_byte(q) ) {
                utf8_yes++;
                i += 6;
                continue;
            }
        }
        utf8_no++;
        i++;
    }
    return (100*utf8_yes)/(utf8_yes + utf8_no);
}

double Encoder::is_gb2312_code(QByteArray str)
{
    unsigned one_byte = 0X00; //binary 00000000
    int gb2312_yes = 0;
    int gb2312_no = 0;
    unsigned char k = 0;
    unsigned char c = 0;
    for (uint i=0; i<str.size();) {
        c = (unsigned char)str.at(i);
        if (c>>7 == one_byte) {
            i++;
            continue;
        } else if (c >= 0XA1 && c <= 0XF7) {
            k = (unsigned char)str.at(i+1);
            if (k >= 0XA1 && k <= 0XFE) {
                gb2312_yes++;
                i += 2;
                continue;
            }
        }
        gb2312_no++;
        i += 2;
    }
    return (100*gb2312_yes)/(gb2312_yes+gb2312_no);
}

double Encoder::is_big5_code(QByteArray str)
{
    unsigned one_byte = 0X00; //binary 00000000
    int big5_yes = 0;
    int big5_no = 0;
    unsigned char k = 0;
    unsigned char c = 0;
   for (uint i=0; i<str.size();) {
        c = (unsigned char)str.at(i);
        if (c>>7 == one_byte) {
            i++;
            continue;
        } else if (c >= 0XA1 && c <= 0XF9) {
            k = (unsigned char)str.at(i+1);
            if ( k >= 0X40 && k <= 0X7E
                    || k >= 0XA1 && k <= 0XFE) {
                big5_yes++;
                i += 2;
                continue;
            }
        }
        big5_no++;
        i += 2;
    }
    return (100*big5_yes)/(big5_yes+big5_no);
}

double Encoder::is_gbk_code(QByteArray str)
{
    unsigned one_byte = 0X00; //binary 00000000
    int gbk_yes = 0;
    int gbk_no = 0;
    unsigned char k = 0;
    unsigned char c = 0;
    for (uint i=0; i<str.size();) {
        c = (unsigned char)str.at(i);
        if (c>>7 == one_byte) {
            i++;
            continue;
        } else if (c >= 0X81 && c <= 0XFE) {
            k = (unsigned char)str.at(i+1);
            if (k >= 0X40 && k <= 0XFE) {
                gbk_yes++;
                i += 2;
                continue;
            }
        }
        gbk_no++;
        i += 2;
    }
    return (100*gbk_yes)/(gbk_yes+gbk_no);
}
