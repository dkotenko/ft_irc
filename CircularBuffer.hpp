#ifndef CIRCULARBUFFER_HPP
# define CIRCULARBUFFER_HPP

# include <iostream>
# include <string>

class CircularBuffer
{

	public:

		CircularBuffer(int size);
		CircularBuffer( CircularBuffer const & src );
		~CircularBuffer();

		CircularBuffer &		operator=( CircularBuffer const & rhs );

	private:
		char *m_buf;
		int m_pos;
		int m_len;
		int m_size;

};

std::ostream &			operator<<( std::ostream & o, CircularBuffer const & i );

#endif /* ************************************************** CIRCULARBUFFER_H */