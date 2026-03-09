//Examples.h


class register_sample
{
public:
	register_sample(const char *name, function<void (void)> f);

private:
	static int n_entry;
};
