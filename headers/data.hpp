#ifndef __DATA_HPP__
# define __DATA_HPP__


class Data
{
private:
	typedef struct	s_form
	{
		std::string		_name;
		std::string		_username;
		std::string		_email;
		std::string		_password;
		size_t			_age;

		std::string		_cookie;
		time_t			_created_date;
		int				_lifespan;
		bool			_alive;

		size_t			_index;

	}Formulaire;
	
	std::map<size_t, Formulaire>	_form;
	size_t							_nb_formulaire;

public:

	Data( void ) : _nb_formulaire(0) {}


	bool	check_login( const std::string &login ) {
		return (find_formulaire(login).first);
	}


	std::string	get_name( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_name : std::string());
	}


	std::string	get_email( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_email : std::string());
	}


	std::string	get_password( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_password : std::string());
	}


	size_t	get_age( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_age : size_t());
	}


	size_t	get_index( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_index : size_t());
	}


	bool	get_alive( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_alive : false);
	}


	std::string	get_cookie( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form ? form->_cookie : std::string());
	}


	int	get_lifespan( const std::string &login ) {
		Formulaire	*form = get_formulaire(login);
		return (form && form->_alive ? form->_lifespan : -1);
	}


	void	set_name( const std::string	&login, const std::string &name ) {
		Formulaire	*form = get_formulaire(login);
		if (form) form->_name = name;
	}


	void	set_email( const std::string &login, const std::string &email ) {
		Formulaire	*form = get_formulaire(login);
		if (form) form->_email = email;
	}


	void	set_password( const std::string	&login, const std::string &password ) {
		Formulaire	*form = get_formulaire(login);
		if (form) form->_password = password;
	}


	void	set_age( const std::string &login, const int age ) {
		Formulaire	*form = get_formulaire(login);
		if (form) form->_age = age;
	}


	void	set_alive( const std::string &login, const bool alive ) {
		Formulaire	*form = get_formulaire(login);
		if (form) form->_alive = alive;
	}


	void	set_cookie( const std::string &login, const std::string &cookie) {
		Formulaire	*form = get_formulaire(login);
		if (form) {
			form->_cookie = cookie;
			form->_created_date = time(NULL);
			form->_lifespan = 300;
			form->_alive = true;
		}
	}


	bool	add_formulaire( const std::string &name, const std::string &username, const std::string &email, \
		const std::string &password, const size_t age)
	{
		if (check_login(username))
			return (false);

		Formulaire new_form;

		new_form._name		= name;
		new_form._username	= username;
		new_form._email		= email;
		new_form._password	= password;
		new_form._age		= age;
		new_form._alive		= false;
		new_form._index		= _nb_formulaire;

		_form.insert(std::make_pair(_nb_formulaire++, new_form));
		return (true);
	}


	bool	delete_formulaire( const std::string &login ) {

		Formulaire	*form = get_formulaire(login);
		if (!form)
			return (false);
		_form.erase(_form.find(form->_index));
		return (true);
	}


private:

	Formulaire*	get_formulaire( const std::string &login ) {
		std::pair<bool, Formulaire*>	pr = find_formulaire(login);
		return (pr.first ? pr.second : NULL);
	}


	std::pair<bool, Formulaire*>	find_formulaire( const std::string &login ) {
		for (std::map<size_t, Formulaire>::iterator it = _form.begin();
			it != _form.end(); ++it) 
		{
			if (it->second._username == login || it->second._cookie == login)
				return (std::pair<bool, Formulaire*>(true, &it->second));
		}
		return (std::pair<bool, Formulaire*>(false, NULL));
	}

};

#endif
