#ifndef ENCODER_H
#define ENCODER_H

#include <QString>

class Encoder {
	public:
		static int is_utf8_special_byte(unsigned char);
		static double is_utf8_code(const QByteArray);
		static double is_gb2312_code(const QByteArray);
		static double is_big5_code(const QByteArray);
		static double is_gbk_code(const QByteArray);
};

#endif // ENCODER_H
