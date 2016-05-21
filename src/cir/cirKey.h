#ifndef KEY
#define KEY
//finction for read operation
class nameKey
{
public:
    nameKey(string s){_name=s;}
    size_t operator() () const{
		size_t sum=0;
		for(size_t i=0;i<_name.size();++i)
			sum+=_name[i];
		return sum;
	}
    bool operator == (const nameKey& k) const { return _name==k._name; }
 private:
 	string _name;
 };


class simKey{
public:
	simKey(unsigned int s){
		if((s&MASK_INVERT)==1)
			_sim=s;
		else
			_sim=~s;
	}
	size_t operator()()const{return _sim;}
	bool operator== (const simKey& k)const {return _sim==k._sim;}

private:
	unsigned int _sim;

};
#endif