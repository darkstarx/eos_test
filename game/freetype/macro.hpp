#ifndef FREETYPE_FTCHECK_HPP
#define FREETYPE_FTCHECK_HPP

#include <utils/log.hpp>

#define FTCHECK(operation) \
{ \
	const FT_Error err = operation; \
	CHECK(!err) << "[FreeType] Ошибка вызова " << #operation << ": " << err; \
}

#define FTLOG(operation) \
{ \
	const FT_Error err = operation; \
	LOG_IF(ERR, err) << "[FreeType] Ошибка вызова " << #operation << ": " << err; \
}

#define FT_PIXEL_26_6(x) ((((x) + 63) & -64) >> 6)

#endif // FREETYPE_FTCHECK_HPP
