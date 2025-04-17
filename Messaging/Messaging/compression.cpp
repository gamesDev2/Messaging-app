#include "compression.h"

zInt::zInt()
{
	// 1 Megabyte buffer (This is perhaps unecessarily large)
	bufferLen = 1024 * 1024;
	pBuf = (unsigned char*)malloc((size_t)bufferLen);
}

zInt::~zInt()
{
	delete pBuf;
}

std::vector<unsigned char> zInt::myCompress(std::vector<unsigned char> _msg)
{
	z_stream dataStream;
	memset(&dataStream, 0, sizeof(dataStream));
	dataStream.next_in = _msg.data();
	dataStream.avail_in = _msg.size();
	dataStream.next_out = pBuf;
	dataStream.avail_out = bufferLen;

	if (deflateInit(&dataStream, Z_BEST_COMPRESSION) != Z_OK)
	{
		throw std::runtime_error("deflateInit() failed");
	}

	std::vector<unsigned char> cmpMsg;

	int status;
	while (dataStream.avail_in)
	{
		status = deflate(&dataStream, Z_FINISH);

		if ((status == Z_STREAM_END) || (!dataStream.avail_out))
		{
			// Output buffer is full or compression done, so we write to output vector
			size_t n = bufferLen - dataStream.avail_out;
			cmpMsg.insert(cmpMsg.end(), pBuf, pBuf + n);

			dataStream.next_out = pBuf;
			dataStream.avail_out = bufferLen;
		}
		else if (status != Z_OK)
		{
			throw std::runtime_error("deflate() failed");
		}
	}
	if (deflateEnd(&dataStream) != Z_OK)
	{
		throw std::runtime_error("deflateEnd() failed");
	}

	return cmpMsg;
}

std::vector<unsigned char> zInt::myDecompress(std::vector<unsigned char> _msg)
{
	// Initialize z_stream and inflate
	z_stream dataStream;
	memset(&dataStream, 0, sizeof(dataStream));
	dataStream.next_in = _msg.data();
	dataStream.avail_in = _msg.size();
	dataStream.next_out = pBuf;
	dataStream.avail_out = bufferLen;

	if (inflateInit(&dataStream))
	{
		throw std::runtime_error("inflateInit() failed");
	}
	std::vector<unsigned char> umpMsg;

	int status;
	while (dataStream.avail_in)
	{
		status = inflate(&dataStream, Z_SYNC_FLUSH);

		if ((status == Z_STREAM_END) || (!dataStream.avail_out))
		{
			// Output buffer is full or compression done, so we write to output vector
			size_t n = bufferLen - dataStream.avail_out;
			umpMsg.insert(umpMsg.end(), pBuf, pBuf + n);

			dataStream.next_out = pBuf;
			dataStream.avail_out = bufferLen;
		}
		else if (status != Z_OK)
		{
			throw std::runtime_error("inflate() failed");
		}
	}

	if (inflateEnd(&dataStream) != Z_OK)
	{
		throw std::runtime_error("inflateEnd() failed");
	}

	return umpMsg;
}
