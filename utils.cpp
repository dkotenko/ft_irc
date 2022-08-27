#include "utils.h"

int xassert(int value, const std::string& err_message)
{
	if (!value) {
		std::cout << err_message << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	return !value ? 1 : 0;
}

void sigh(int n) {
	(void)n;
    exit(0);
}

void fct_read(struct s_env *env, int fd) {
	if (env->fds[fd].type == FD_SERV) {
		srv_accept(env, fd);
	} else if (env->fds[fd].type == FD_CLIENT) {
		client_read(env, fd);
	} else {
		std::cout << "Error: invalid fd type in fct_read: " << env->fds[fd].type << std::endl;
	}
}

void srv_accept(struct s_env *env, int s)
{
  int			cs;
  struct sockaddr_in	csin;
  socklen_t		csin_len;

  csin_len = sizeof(csin);
  cs = accept(s, (struct sockaddr*)&csin, &csin_len);
  printf("New client #%d from %s:%d\n", cs,
	 inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
  env->fds[cs].type = FD_CLIENT;
}

void client_read(struct s_env *env, int cs)
{
  int	r;
  int	i;

  r = recv(cs, env->fds[cs].buf_read, BUF_SIZE, 0);
  if (r <= 0)
    {
      close(cs);
      env->fds[cs].clean();
      printf("client #%d gone away\n", cs);
    }
  else
    {
      i = 0;
      while (i < env->maxfd)
	{
	  if ((env->fds[i].type == FD_CLIENT) &&
	      (i != cs))
	    send(i, env->fds[cs].buf_read, r, 0);
	  i++;
	}
    }
}

void fct_write(struct s_env *env, int cs)
{
	client_write(env, cs);
}

void client_write(struct s_env *env, int cs)
{
	(void)env;
    (void)cs;
}
