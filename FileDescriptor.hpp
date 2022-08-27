#ifndef FILEDESCRIPTOR_HPP
# define FILEDESCRIPTOR_HPP

# include <iostream>
# include <string>

# define BUF_SIZE	4096
# define FD_FREE	0
# define FD_SERV	1
# define FD_CLIENT	2

class FileDescriptor
{

	public:

		FileDescriptor();
		//FileDescriptor( FileDescriptor const & src );
		~FileDescriptor();
		int GetType() const;

		//FileDescriptor &		operator=( FileDescriptor const & rhs );
		
		/*
		void fct_read(struct s_env *env, int fd);
		void fct_write(struct s_env *env, int fd);
		void	srv_accept(struct s_env *env, int fd);
		void	client_read(struct s_env *env, int cs);
		void	client_write(struct s_env *env, int cs);
		*/
		
		int type;
		char buf_read[BUF_SIZE + 1];
		char buf_write[BUF_SIZE + 1];
		void clean();
	private:
};

std::ostream &			operator<<( std::ostream & o, FileDescriptor const & i );

#endif /* ************************************************** FILEDESCRIPTOR_H */
