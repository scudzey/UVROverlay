template<class T>
class NeedsObject {
	T object;
public:
	NeedsObject(T object) : object(object) { }
};