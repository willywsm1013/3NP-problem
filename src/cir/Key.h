#ifndef CIRKEY
#define CIRKEY
//finction for read operation
#define MASK_INVERT (size_t)0x1
#define MASK_AIG (size_t)0x2
class simKey{
public:
	simKey(unsigned int s){
		if((s & MASK_INVERT)==1)
			_sim=s;
		else
			_sim=~s;
	}
	size_t operator()()const{return _sim;}
	bool operator== (const simKey& k)const {return _sim==k._sim;}

private:
	unsigned int _sim;

};

class nameKey{
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



#endif
