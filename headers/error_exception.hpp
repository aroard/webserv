#ifndef __ERROR_EXCEPTION_HPP__
# define __ERROR_EXCEPTION_HPP__


class Error_exception {

private:

	const std::string	_msg_err;
	const int			_error_code;

public:

	Error_exception( const std::string &msg_err, const int error_code ) 
		: _msg_err(msg_err), _error_code(error_code) {}
	
	static void socket_close( int signum ) { throw Error_exception("Socket is closing", 1); (void)signum; }
	
	static void interruption_server( int signum ) { throw Error_exception("\rServer down", 0); (void)signum; }
	
	static void error( const std::string &msg_err, const int error_code ) { 
		throw Error_exception(msg_err, error_code);
	}
	const std::pair<std::string, int>	what( void ) const { 
		return (std::pair<std::string, int>(_msg_err, _error_code)); 
	}

};


#endif
