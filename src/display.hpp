
#include <iostream>

class Display(){

private:


public:

	//canonical form
	Display(void);
	~Display(void);
	Display(const Display & _display)
	Display & operator=(const Display & _rhs);

	//initialise quit and cleanup functions
	virtual void Init(void) = 0;
	virtual void quit(void) = 0;
	virtual int  destroy() = 0;

	virtual void Update() = 0;
	virtual void draw() = 0;
};
