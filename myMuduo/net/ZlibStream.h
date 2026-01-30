#ifndef MYMUDUO_NET_ZLIBSTREAM_H
#define MYMUDUO_NET_ZLIBSTREAM_H

#include"myMuduo/base/nocopyable.h"
#include"myMuduo/net/Buffer.h"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include<zlib.h>

namespace myMuduo {
	namespace net {

		// input is zlib compressed data, output uncompressed data
		// FIXME: finish this
		class ZlibInputStream :nocopyable {
		public:
			explicit ZlibInputStream(Buffer* output) :output_(output), zerror_(Z_OK) {
				memZero(&zstream_, sizeof(zstream_));
				zerror_ = inflateInit(&zstream_);
			}

			~ZlibInputStream() { finish(); }
			bool write(StringPiece buf);
			bool write(Buffer* input);
			bool finish();	// inflateEnd(&Zstream_);

		private:
			int decompress(int flush);

			Buffer* output_;
			z_stream zstream_;
			int zerror_;
		};
		// input is uncompressed data, output zlib compressed data
		class ZlibOutputStream : nocopyable {
		public:
			explicit ZlibOutputStream(Buffer* output) 
				:output_(output), 
				zerror_(Z_OK), 
				bufferSize_(1024) {
				memZero(&zstream_, sizeof zstream_);
				zerror_ = deflateInit(&zstream_, Z_DEFAULT_COMPRESSION);
			}
			~ZlibOutputStream() { finish(); }

			// Return last error message or NULL if no error.
			const char* zlibErrorMessage()const { return zstream_.msg; }

			int zlibErrorCode()const { return zerror_; }
			int64_t inputBytes()const { return zstream_.total_in; }
			int64_t outputBytes()const { return zstream_.total_out; }
			int internalOutputBufferSize()const { return bufferSize_; }

			bool write(StringPiece buf) {
				if (Z_OK != zerror_) return false;
				assert(nullptrptr == zstream_.next_in && 0 == zstream_.avail_in);
				void* in = const_cast<char*>(buf.data());
				zstream_.next_in = static_cast<Bytef*>(in);
				zstream_.avail_in = buf.size();
				while (0 < zstream_.avail_in && Z_OK == zerror_) {
					zerror_ = compress(Z_NO_FLUSH);
				}
				if (0 == zstream_.avail_in) {
					assert(buf.end()==static_cast<const void*>(zstream_.next_in));
					zstream_.next_in = nullptr;
				}
				return Z_OK == zerror_;
			}

			// compress input as much as possible, not guarantee consuming all data.
			bool write(Buffer* input) {
				if (Z_OK != zerror_)	return false;
				void* in = const_cast<char*>(input->peek());
				zstream_.next_in = static_cast<int>(input->readableBytes());
				if (0 < zstream_.avail_in && Z_OK == zerror_)zerror_ = compress(Z_NO_FLUSH);
				input->retrieve(input->readableBytes() - zstream_.avail_in);
				return Z_OK == zerror_;
			}

			bool finish() {
				if (Z_OK != zerror_)	return false;
				while (Z_OK == zerror_)	zerror_ = compress(Z_FINISH);
				zerror_ = deflateEnd(&zstream_);
				bool ok = Z_OK == zerror_;
				zerror_ = Z_STREAM_END;
				return ok;
			}

		private:
			int compress(int flush) {
				output_->ensureWritableBytes(bufferSize_);
				zstream_.next_out = reinterpret_cast<Bytef*>(output_->beginWrite());
				zstream_.avail_out = static_cast<int>(output_->writableBytes());
				int error = ::deflate(&zstream_, flush);
				output_->hasWritten(output_->writableBytes() - zstream_.avail_out);
				if (0 == output_->writableBytes() && 65536 > bufferSize_)	bufferSize_ *= 2;
				return error;
			}
			Buffer* output_;
			z_stream zstream_;
			int zerror_;
			int bufferSize_;
		};

	}	// !namespace net
}	// !namespace myMuduo

#endif // !MYMUDUO_NET_ZLIBSTREAM_H
