#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <algorithm>

template<class T>
class btree
{
	typedef std::size_t size_type;

	class internal;

	class node
	{
	public:
		virtual void split()=0;
		virtual void insert(const T& d)=0;
		virtual void erase(const T& d)=0;
		virtual node* search(const T& d)=0;
		virtual void deltree()=0;
		virtual void print() const=0;
		virtual void print_histogram() const=0;
		virtual void print_keys(unsigned) const=0;
		virtual void set_parent(internal*)=0;
	};

	class internal:
		public node
	{
	public:
		friend class btree;
		friend class node;
		friend class leaf;

		internal(size_type _m, btree<T>* enc): m(_m), enclosing(enc), parent(nullptr) {}
		void set_parent(internal* n)
		{
			parent = n;
		}
		void split()
		{
			if(keys.size() > m)
			{
				internal* nint = new internal(m, enclosing);
				T midkey = *(keys.begin()+(m/2));
				for(typename std::vector<T>::iterator i = keys.begin()+(m/2)+1; i != keys.end(); ++i)
				{
					nint->keys.push_back(*i);
				}
				keys.erase(keys.begin()+(m/2), keys.end());

				for(typename std::vector<node*>::iterator i = children.begin()+((m+1)/2); i != children.end(); ++i)
				{
					(*i)->set_parent(nint);
					nint->children.push_back(*i);
				}
				children.erase(children.begin()+((m+1)/2), children.end());

				if(parent == nullptr)
				{
					internal* nroot = new internal(m, enclosing);
					nroot->children.push_back(this);

					this->parent = nroot;

					enclosing->root = nroot;
				}

				typename std::vector<T>::iterator i = std::lower_bound(parent->keys.begin(), parent->keys.end(), midkey);
				if(i == parent->keys.end())
				{
					parent->children.push_back(nint);
					parent->keys.push_back(midkey);
				}
				else
				{
					auto index = std::distance(parent->keys.begin(), i);
					parent->children.insert(parent->children.begin()+index+1, nint);
					parent->keys.insert(i, midkey);
				}

				nint->parent = this->parent;

				parent->split();
			}
		}
		
		void insert(const T& d)
		{
			typename std::vector<T>::iterator i = std::lower_bound(keys.begin(), keys.end(), d);
			if(i == keys.end())
			{
				// call insertion of data d in node at end of parent child vector
				(*(children.end()-1))->insert(d);
			}
			else
			{
				// get the distance to the key
				auto index = std::distance(keys.begin(), i);
				// call insertion on that node
				(*(children.begin()+index))->insert(d);
			}
		}
		
		void erase(const T& d)
		{
			if(search(d) != nullptr)
				search(d)->erase(d);
		}
		
		node* search(const T& d)
		{
			typename std::vector<T>::iterator i = std::lower_bound(keys.begin(), keys.end(), d);
			if(i == keys.end())
				return ((*(children.end()-1))->search(d));
			else
			{
				auto index = std::distance(keys.begin(), i);
				return ((*(children.begin()+index))->search(d));
			}
		}

		void deltree()
		{
			for(typename std::vector<node*>::iterator i = children.begin(); i != children.end(); ++i)
			{
				(*i)->deltree();
			}
			delete this;
		}

		void print() const
		{
			for(typename std::vector<node*>::const_iterator i = children.begin(); i != children.end(); ++i)
			{
				(*i)->print();
			}
		}

		void print_histogram() const
		{
			for(typename std::vector<node*>::const_iterator i = children.begin(); i != children.end(); ++i)
			{
				(*i)->print_histogram();
			}
		}

		void print_keys(unsigned height) const
		{
			std::cout << "Node at height: " << height++ << '\n';
			std::cout << "Keys: ";
			for(typename std::vector<T>::const_iterator i = keys.begin(); i != keys.end(); ++i)
			{
				std::cout << *i << ' ';
			}
			std::cout << '\n';

			for(typename std::vector<node*>::const_iterator i = children.begin(); i != children.end(); ++i)
			{
				(*i)->print_keys(height);
			}
		}

	private:
		btree<T>* enclosing;
		const size_type m;

		internal* parent;
		std::vector<T> keys;
		std::vector<node*> children;
	};

	class leaf:
		public node
	{
	public:
		friend class btree;
		friend class node;
		friend class internal;

		leaf(size_type _m, btree<T>* enc): m(_m), enclosing(enc), parent(nullptr) {}
		
		void set_parent(internal* n)
		{
			parent = n;
		}

		void split()
		{
			if(data.size() > m)
			{
				leaf* nleaf = new leaf(m, enclosing);
				T midkey = *(data.begin()+(m/2));
				for(typename std::vector<T>::iterator i = data.begin()+(m/2)+1; i != data.end(); ++i)
				{
					nleaf->data.push_back(*i);
				}
				data.erase(data.begin()+(m/2)+1, data.end());

				if(parent == nullptr)
				{
					internal* ninternal = new internal(m, enclosing);
					ninternal->children.push_back(this);

					this->parent = ninternal;

					enclosing->root = ninternal;
				}

				typename std::vector<T>::iterator i = std::lower_bound(parent->keys.begin(), parent->keys.end(), midkey);
				if(i == parent->keys.end())
				{
					parent->children.push_back(nleaf);
					parent->keys.push_back(midkey);
				}
				else
				{
					auto index = std::distance(parent->keys.begin(), i);
					parent->children.insert(parent->children.begin()+index+1, nleaf);
					parent->keys.insert(i, midkey);
				}

				nleaf->parent = this->parent;

				parent->split();
			}
		}
		void insert(const T& d)
		{
			typename std::vector<T>::iterator i = std::lower_bound(data.begin(), data.end(), d);
			data.insert(i, d);
			split();
		}
		void erase(const T& d)
		{
			for(typename std::vector<T>::iterator i = data.begin(); i != data.end(); ++i)
			{
				if(*i == d)
				{
					data.erase(i);
					return;
				}
			}
		}
		node* search(const T& d)
		{
			for(typename std::vector<T>::iterator i = data.begin(); i != data.end(); ++i)
			{
				if(*i == d) return this;
			}
			return nullptr;
		}

		void deltree()
		{
			delete this;
		}

		void print() const
		{
			for(typename std::vector<T>::const_iterator i = data.begin(); i != data.end(); ++i)
			{
				std::cout << *i << ' ';
			}
			std::cout << "| ";
		}

		void print_histogram() const
		{
			for(typename std::vector<T>::const_iterator i = data.begin(); i != data.end(); ++i)
			{
				std::cout << "x ";
			}
			std::cout << '\n';
		}

		void print_keys(unsigned a) const
		{
			std::cout << "Leaf at height: " << a << "\n| ";
			print();
			std::cout << '\n';
		}

	private:
		btree<T>* enclosing;
		const size_type m;

		internal* parent;
		std::vector<T> data;
	};

public:
	btree(size_type _m): m(_m), root(new leaf(_m, this)) {}

	void insert(const T&);
	void erase(const T&);
	void deltree(node*);
	void clear();
	node* search(const T&) const;

	void print() const;
	void print_histogram() const;
	void print_keys() const;
	std::vector<T>& data_to_vector() const;

	~btree();

private:
	node* root;
	const size_type m;
};

template<class T>
btree<T>::~btree()
{
	clear();
	delete this->root;
}

template<class T>
void btree<T>::insert(const T& d)
{
	this->root->insert(d);
}

template<class T>
void btree<T>::erase(const T& d)
{
	this->root->erase(d);
}

template<class T>
void btree<T>::deltree(node* n)
{
	n->deltree();
}

template<class T>
void btree<T>::clear()
{
	deltree(this->root);
	this->root = new leaf(m, this);
}

template<class T>
void btree<T>::print() const
{
	this->root->print();
}

template<class T>
void btree<T>::print_histogram() const
{
	this->root->print_histogram();
}

template<class T>
void btree<T>::print_keys() const
{
	this->root->print_keys(0);
}

template<class T>
typename btree<T>::node* btree<T>::search(const T& d) const
{
	return this->root->search(d);
}

#endif