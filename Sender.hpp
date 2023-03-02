#pramga once

# include <iostream>
# include <string>

class Sender
{

	public:

		Sender();
		Sender( Sender const & src );
		~Sender();

		Sender &		operator=( Sender const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Sender const & i );

#endif /* ********************************************************** SENDER_H */