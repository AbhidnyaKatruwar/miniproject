size_t f_write( void *buffer, size_t size, size_t num, FILE *stream) {
   char *data;               // Point to where data comes from next
  unsigned total;                 // Total bytes to write
  unsigned count;                 // Num bytes left to write
  unsigned bufsize;               // Size of stream buffer
  unsigned nbytes;                // Number of bytes to write now
  unsigned nwritten;              // Number of bytes written
  int c;                          // A temp char

  // Initialize local vars
  data = buffer;
  count = total = size * num;
  if (count == 0) 
	return 0;
	bufsize = BUFSIZ;
  

  // Here is the main loop -- we go through here until we're done
  while (count != 0) {
    // If the buffer is big and has room, copy data to buffer
    if (stream -> cnt != 0) {
      // How much do we want?
      nbytes = (count < (unsigned) stream->cnt) ? count : stream->cnt;
      memcpy(stream->ptr, data, nbytes);

      // Update stream and amount of data written
      count -= nbytes;
      stream -> cnt -= nbytes;
      stream -> ptr += nbytes;
      data += nbytes;
    } else if (count >= bufsize) {
      // If we have more than bufsize chars to write, write
      // data by calling write with an integral number of
      // bufsiz blocks.  If we reach here and we have a big
      // buffer, it must be full so flush it.

    
      // Calc chars to write -- (count / bufsize) * bufsize
      nbytes = bufsize ? (count - count % bufsize) : count;

      if (stream->flag & _IOCRLF) {
        nwritten = write_translated(fileno(stream), (char *) data, nbytes);
      } else {
        nwritten = write(fileno(stream), data, nbytes);
      }

      if ((int) nwritten < 0) {
        // Error -- out of here
        stream->flag |= _IOERR;
        return (total - count) / size;
      }

      // Update count and data to reflect write
      count -= nwritten;
      data += nwritten;

      if (nwritten < nbytes) {
        // Error -- out of here
        stream->flag |= _IOERR;
        return (total - count) / size;
      }
    } else {
      // Buffer full and not enough chars to do direct write, so do a flsbuf.
      c = *data;  
      if (flsbuf(c, stream) == EOF) {
        // Error or eof, stream flags set by _flsbuf
        return (total - count) / size;
      }

      // flsbuf wrote a char -- update count
      ++data;
      --count;

      // Update buffer size
      bufsize = stream->bufsiz > 0 ? stream->bufsiz : 1;
    }
  }

  // We finished successfully, so just return num
  return num;
}


