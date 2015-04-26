/*
 * lcd_typewriter.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <memory>
#include <vector>

#include <libbase/log.h>
#include <libbase/misc_types.h>

#include <libsc/config.h>
#include <libsc/lcd.h>
#include "MyTypeWriter.h"

using namespace std;
using namespace libsc;

MyTypeWriter::MyTypeWriter(const Config &config)
		: m_lcd(config.lcd),
		  m_fg_color(config.text_color),
		  m_bg_color(config.bg_color),
		  m_is_clear_line(config.is_clear_line),
		  kCursorX(0),
		  kCursorY(0)
{
	assert(config.lcd);
	m_lcd->Clear(m_bg_color);
}

bool MyTypeWriter::setActiveRegion(const uint8_t cursorX, const uint8_t cursorY)
{
	if (checkInRange(0, cursorX, kMaxColumn) && checkInRange(0, cursorY, kMaxRow))
	{
		m_lcd->SetRegion(Lcd::Rect(cursorX * 6, cursorY * 10, 6, 10));
		return true;
	}
	return false;
}

void MyTypeWriter::WriteChar(const char c)
{
	if (c == '\n')
	{
		kCursorY++;
		kCursorX = 0;
		return ;
	}
	WriteChar(CharTable_Size5[c - 32]);
}

void MyTypeWriter::WriteChar(const uint8_t dataArray[10])
{
	if (!setActiveRegion(kCursorX++, kCursorY))
		return ;

	bool bitArray[6 * 10] = { false };

	for (int8_t y = 0; y < 10; y++)
		for (int8_t x = 0; x < 6; x++)
			bitArray[6 * y + x] = (bool)((dataArray[y] >> (5 - x)) & 1);

	m_lcd->FillBits(m_fg_color, m_bg_color, bitArray, 6 * 10);

	m_lcd->ClearRegion();
}

void MyTypeWriter::WriteString(const char *str)
{
	WriteBuffer(str, strlen(str));
}

void MyTypeWriter::WriteBuffer(const char *buf, const size_t length)
{
	if (length == 0)
	{
		return;
	}

	uint currentIndex = 0;
	while (currentIndex < length)
	{
		for (uint8_t x = kCursorX; x < kMaxColumn && currentIndex < length; x++, currentIndex++)
		{
			if (buf[currentIndex] == '\n')
			{
				if (m_is_clear_line)
					for (int8_t i = 0; i < kMaxColumn - kCursorX; i++)
						WriteChar(CharTable_Size5[0]);

				kCursorX = 0;
				kCursorY++; currentIndex++;
				break;
			}
			else if (checkInRange(31, buf[currentIndex], 126))
				WriteChar(CharTable_Size5[buf[currentIndex] - 32]);
			else
				WriteChar(CharTable_Size5[0]);
		}
	}
//	size_t start = 0;
//	size_t count = 0;
//	const size_t max_count = std::max<size_t>(region.w / kFontW, 1);
//	size_t y = region.y;
//	size_t h = region.h;
//	size_t print = 0;
//	while (print < length)
//	{
//		if (buf[print] == '\n' || (m_is_text_wrap && count == max_count))
//		{
//			m_lcd->SetRegion({region.x, y, region.w, h});
//			WriteOneLineBuffer(buf + start, count);
//			count = 0;
//			y += kFontH;
//			h -= kFontH;
//			if (buf[print] == '\n')
//			{
//				start = print + 1;
//				++print;
//			}
//			else
//			{
//				start = print;
//			}
//		}
//		else
//		{
//			++count;
//			++print;
//		}
//	}
	// Last line
//	if (count > 0)
//	{
//		m_lcd->SetRegion({region.x, y, region.w, h});
//		WriteOneLineBuffer(buf + start, count);
//	}


//	m_lcd->SetRegion(region);
}
//
//void MyTypeWriter::WriteOneLineBuffer(const char *buf, const size_t length)
//{
//	if (length == 0)
//	{
//		return;
//	}
//
//	unique_ptr<const Byte*> font_data(new const Byte*[length]);
//	for (size_t i = 0; i < length; ++i)
//	{
//		if (buf[i] < 32 || buf[i] > 126)
//		{
//			LOG_WL("Unsupported char");
//			font_data.get()[i] = &CharTable_Size5[0][0];
//		}
//		else
//		{
//			font_data.get()[i] = &CharTable_Size5[buf[i] - 32][0];
//		}
//	}
//	const Lcd::Rect &region = m_lcd->GetRegion();
//	const Uint w = std::min<Uint>(region.w, kFontW * length);
//	const Uint full_w = m_is_clear_line ? region.w : w;
//	const Uint h = std::min<Uint>(region.h, kFontH);
//	if (w == 0 || h == 0)
//	{
//		return;
//	}
//	const Uint pixel_count = full_w * h;
//	vector<Byte> bitset((pixel_count + 7) / 8, 0);
//	Byte *bs_ptr = &bitset[0];
//	Uint filled_bit = 0;
//
//	Uint row_remaining_pixel = full_w - w;
//
//	// Won't work if kFontW != 8 BTW
//	Uint font_pos = 0;
//	for (Uint y = 0; y < h; ++y)
//	{
//		for (Uint x = 0; x < w; x += 8)
//		{
//			const Uint bit_count = std::min<Uint>(w - x, 8);
//			const uint8_t bit_mask = 0xFF << (8 - bit_count);
//
//			*bs_ptr |= (*font_data.get()[font_pos] & bit_mask) >> filled_bit;
//
//			if (filled_bit + bit_count > 8)
//			{
//				// Fill the remaining bits
//				*++bs_ptr |= (*font_data.get()[font_pos] & bit_mask)
//						<< (8 - filled_bit);
//				filled_bit = (filled_bit + bit_count) % 8;
//			}
//			else if (filled_bit + bit_count == 8)
//			{
//				++bs_ptr;
//				filled_bit = 0;
//			}
//			else
//			{
//				filled_bit += bit_count;
//			}
//			++font_data.get()[font_pos];
//			++font_pos;
//		}
//		font_pos = 0;
//
//		if (m_is_clear_line)
//		{
//			for (Uint i = 0; i < row_remaining_pixel; ++i)
//			{
//				if (++filled_bit == 8)
//				{
//					filled_bit = 0;
//					++bs_ptr;
//				}
//			}
//		}
//	}
//	m_lcd->SetRegion({region.x, region.y, full_w, h});
//	m_lcd->FillBits(m_fg_color, m_bg_color, bitset.data(), pixel_count);
//	m_lcd->SetRegion(region);
//}


/* (?>\,|\{)\ */
//static const uint8_t CharTable_Size5[MAX_ASCII_INDEX - MIN_ASCII_INDEX][8] =
const uint8_t MyTypeWriter::CharTable_Size5[][10] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 4, 4, 4, 4, 4, 0, 0, 4, 0 },
	{ 0, 27, 18, 18, 0, 0, 0, 0, 0, 0 },
	{ 0, 5, 5, 10, 31, 10, 31, 10, 20, 0 },
	{ 0, 2, 7, 8, 8, 7, 9, 14, 2, 0 },
	{ 0, 8, 20, 8, 3, 28, 2, 5, 2, 0 },
	{ 0, 12, 18, 22, 12, 8, 21, 18, 13, 0 }, // &
	{ 0, 2, 2, 2, 2, 0, 0, 0, 0, 0 },
	{ 0, 2, 2, 4, 4, 4, 4, 2, 2, 0 }, // '('
	{ 0, 8, 8, 4, 4, 4, 4, 8, 8, 0 },
	{ 0, 4, 31, 4, 10, 10, 0, 0, 0, 0 },
	{ 0, 0, 0, 4, 4, 31, 4, 4, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 6, 4, 12, 8 },
	{ 0, 0, 0, 0, 0, 31, 0, 0, 0, 0 }, // '-'
	{ 0, 0, 0, 0, 0, 0, 0, 12, 12, 0 },
	{ 0, 1, 1, 2, 2, 4, 4, 8, 8, 0 },
	{ 0, 14, 17, 19, 21, 21, 25, 17, 14, 0 }, // '0'
	{ 0, 12, 20, 4, 4, 4, 4, 4, 31, 0 },
	{ 0, 14, 17, 1, 2, 4, 8, 17, 31, 0 },
	{ 0, 14, 17, 1, 6, 1, 1, 17, 14, 0 },
	{ 0, 6, 10, 10, 18, 18, 31, 2, 7, 0 },
	{ 0, 31, 16, 16, 30, 1, 1, 17, 14, 0 }, // '5'
	{ 0, 7, 8, 16, 30, 17, 17, 17, 14, 0 },
	{ 0, 31, 17, 1, 2, 2, 2, 4, 4, 0 },
	{ 0, 14, 17, 17, 14, 17, 17, 17, 14, 0 },
	{ 0, 14, 17, 17, 17, 15, 1, 2, 28, 0 }, // ':'
	{ 0, 0, 0, 12, 12, 0, 0, 12, 12, 0 },
	{ 0, 0, 0, 12, 12, 0, 0, 12, 24, 16 },
	{ 0, 0, 1, 6, 8, 16, 8, 6, 1, 0 },
	{ 0, 0, 0, 0, 31, 0, 31, 0, 0, 0 },
	{ 0, 0, 16, 12, 2, 1, 2, 12, 16, 0 },
	{ 0, 14, 17, 17, 2, 4, 4, 0, 4, 0 },
	{ 0, 14, 17, 19, 21, 19, 16, 17, 14, 0 },
	{ 0, 4, 4, 10, 10, 10, 31, 17, 17, 0 }, // 'A'
	{ 0, 30, 17, 17, 30, 17, 17, 17, 30, 0 },
	{ 0, 14, 17, 16, 16, 16, 16, 17, 14, 0 },
	{ 0, 28, 18, 17, 17, 17, 17, 18, 28, 0 },
	{ 0, 31, 16, 16, 28, 16, 16, 16, 31, 0 },
	{ 0, 31, 16, 16, 28, 16, 16, 16, 16, 0 }, // 'F'
	{ 0, 14, 17, 16, 16, 19, 17, 17, 15, 0 },
	{ 0, 17, 17, 17, 31, 17, 17, 17, 17, 0 },
	{ 0, 14, 4, 4, 4, 4, 4, 4, 14, 0 },
	{ 0, 2, 2, 2, 2, 18, 18, 18, 12, 0 },
	{ 0, 17, 17, 18, 20, 28, 18, 17, 17, 0 }, // 'K'
	{ 0, 16, 16, 16, 16, 16, 16, 16, 31, 0 },
	{ 0, 17, 27, 27, 21, 21, 17, 17, 17, 0 },
	{ 0, 17, 25, 25, 21, 21, 21, 19, 17, 0 },
	{ 0, 14, 17, 17, 17, 17, 17, 17, 14, 0 },
	{ 0, 30, 17, 17, 17, 30, 16, 16, 16, 0 }, // 'P'
	{ 0, 14, 17, 17, 17, 17, 17, 14, 7, 0 },
	{ 0, 30, 17, 17, 17, 30, 18, 17, 17, 0 },
	{ 0, 14, 17, 16, 14, 1, 1, 17, 14, 0 },
	{ 0, 31, 4, 4, 4, 4, 4, 4, 4, 0 },
	{ 0, 17, 17, 17, 17, 17, 17, 17, 14, 0 },
	{ 0, 17, 17, 17, 10, 10, 10, 4, 4, 0 },
	{ 0, 17, 17, 17, 21, 21, 21, 21, 10, 0 },
	{ 0, 17, 17, 10, 4, 4, 10, 17, 17, 0 },
	{ 0, 17, 17, 10, 10, 4, 4, 4, 4, 0 },
	{ 0, 31, 17, 2, 4, 4, 8, 17, 31, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

