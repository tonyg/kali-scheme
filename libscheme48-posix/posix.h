
extern mode_t		s48_extract_mode(s48_value sch_mode);
extern int		s48_extract_file_options(s48_value sch_file_options);

extern s48_value	s48_enter_uid(uid_t uid);
extern uid_t		s48_extract_uid(s48_value uid);

extern s48_value	s48_enter_gid(gid_t gid);
extern gid_t		s48_extract_gid(s48_value gid);

extern s48_value	s48_enter_pid(pid_t c_pid);
