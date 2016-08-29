#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <exception>
#include <vector>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::runtime_error;
using std::unordered_map;
using std::unordered_set;
using std::map;

#define ptr shared_ptr
#define mk make_shared

struct node {
	typedef ptr<const node> ref;
	typedef std::vector<ref> refs;

	virtual ~node() {}
	virtual int get_value() const = 0;
	virtual string str() const = 0;
	virtual int count_fours() const = 0;
	virtual int count_ops() const = 0;
};

struct four : public node {
	int get_value() const { return 4; }
	string str() const { return "4"; }
	int count_fours() const { return 1; }
	int count_ops() const { return 0; }
};

struct negate : public node {
	negate(ref left) : left(left) {}
	static ref create(ref left) {
		return mk<negate>(left);
	}
	int get_value() const {
	   	return -left->get_value();
   	}

	string str() const {
	   	return std::string("(-") + left->str() + ")";
   	}
	int count_fours() const { return left->count_fours(); }
	int count_ops() const { return 1 + left->count_ops(); }
	ref left;
};

int fac(int x) {
	if (x < 0) {
		throw runtime_error("negative factorial");
	} else if (x <= 1) {
		return 1;
	} else {
		return fac(x - 1) * x;
	}
}

struct factorial : public node {
	factorial(ref left) : left(left) {}
	static ref create(ref left) {
		return mk<factorial>(left);
	}
	int get_value() const {
	   	return fac(left->get_value());
   	}

	string str() const {
	   	return std::string("(") + left->str() + "!)";
   	}
	int count_fours() const { return left->count_fours(); }
	int count_ops() const { return 1 + left->count_ops(); }
	ref left;
};

struct sqroot : public node {
	sqroot(ref left) : left(left) {}
	static ref create(ref left) {
		return mk<sqroot>(left);
	}
	int get_value() const {
		float lvalue = left->get_value();
		if (lvalue >= 0) {
			int value = int(sqrt(left->get_value()) + 0.5f);
			if (value * value == lvalue) {
				return sqrt(lvalue);
			} else {
				throw runtime_error("does not square root to an integer");
			}
		} else {
			throw runtime_error("imaginary number");
		}
   	}

	string str() const {
	   	return std::string("√(") + left->str() + ")";
   	}
	int count_fours() const { return left->count_fours(); }
	int count_ops() const { return 1 + left->count_ops(); }
	ref left;
};

struct times : public node {
	times(ref left, ref right) : left(left), right(right) {}
	static ref create(ref left, ref right) {
		return mk<times>(left, right);
	}
	int get_value() const {
	   	return left->get_value() * right->get_value();
   	}

	string str() const {
	   	return std::string("(") + left->str() + " * " + right->str() + ")";
   	}
	int count_fours() const {
	   	return left->count_fours() + right->count_fours();
   	}
	int count_ops() const {
	   	return 1 + left->count_ops() + right->count_ops();
	}
	ref left, right;
};

struct divide : public node {
	divide(ref left, ref right) : left(left), right(right) {}
	static ref create(ref left, ref right) {
		return mk<divide>(left, right);
	}
	int get_value() const {
		int lvalue = left->get_value();
		int rvalue = right->get_value();
		if (rvalue <= 0 || lvalue % rvalue != 0) {
			throw runtime_error("not divisible");
		}
		return lvalue / rvalue;
   	}

	string str() const {
	   	return std::string("(") + left->str() + " / " + right->str() + ")";
   	}
	int count_fours() const {
	   	return left->count_fours() + right->count_fours();
   	}
	int count_ops() const {
	   	return 1 + left->count_ops() + right->count_ops();
	}
	ref left, right;
};

struct plus : public node {
	plus(ref left, ref right) : left(left), right(right) {}
	static ref create(ref left, ref right) {
		return mk<plus>(left, right);
	}
	int get_value() const {
	   	return left->get_value() + right->get_value();
   	}

	string str() const {
	   	return std::string("(") + left->str() + " + " + right->str() + ")";
   	}
	int count_fours() const {
	   	return left->count_fours() + right->count_fours();
   	}
	int count_ops() const {
	   	return 1 + left->count_ops() + right->count_ops();
	}
	ref left, right;
};

struct minus : public node {
	minus(ref left, ref right) : left(left), right(right) {}
	static ref create(ref left, ref right) {
		return mk<minus>(left, right);
	}
	int get_value() const {
	   	return left->get_value() - right->get_value();
   	}

	string str() const {
	   	return std::string("(") + left->str() + " - " + right->str() + ")";
   	}
	int count_fours() const {
	   	return left->count_fours() + right->count_fours();
   	}
	int count_ops() const {
	   	return 1 + left->count_ops() + right->count_ops();
	}
	ref left, right;
};

node::ref base = mk<four>();

using permute_func = std::function<node::refs (node::ref, node::refs)>;

bool is_a_keeper(const unordered_set<string> &seen, node::ref key) {
	// TODO: bucketize each key by whether another key shares it's value/count combo
	int value = key->get_value();
	int count_fours = key->count_fours();
	int count_ops = key->count_ops();

	if (abs(value) > 10000) {
		return false;
	}

	if (count_fours > 4) {
		return false;
	}

	if (count_ops > 20) {
		return false;
	}

	return seen.find(key->str()) == seen.end();
}

int main(int argc, char *argv[]) {
	unordered_set<string> seen;
	std::vector<node::ref> trees;

	auto permutations = std::vector<permute_func>{
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the plus operation, which only needs one direction */
			for (auto other : others) {
				try {
					node::ref new_ref = plus::create(key, other);
					if (is_a_keeper(seen, new_ref)) {
						/* we found an easier way to make this value */
						seen.insert(new_ref->str());
						new_refs.push_back(new_ref);
					}
				} catch (const std::exception &) {
				}
			}
			return new_refs;
		},
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the minus operation, which needs two directions */
			for (auto other : others) {
				try {
					node::ref new_ref = minus::create(key, other);
					if (is_a_keeper(seen, new_ref)) {
						/* we found an easier way to make this value */
						seen.insert(new_ref->str());
						new_refs.push_back(new_ref);
					}
				} catch (const std::exception &) {
				}
				try {
					node::ref new_ref = minus::create(other, key);
					if (is_a_keeper(seen, new_ref)) {
						/* we found an easier way to make this value */
						seen.insert(new_ref->str());
						new_refs.push_back(new_ref);
					}
				} catch (const std::exception &) {
				}
			}
			return new_refs;
		},
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the divide operation, which needs two directions */
			for (auto other : others) {
				try {
					node::ref new_ref = divide::create(key, other);
					if (is_a_keeper(seen, new_ref)) {
						/* we found an easier way to make this value */
						seen.insert(new_ref->str());
						new_refs.push_back(new_ref);
					}
				} catch (const std::exception &) {
				}
				try {
					node::ref new_ref = divide::create(other, key);
					if (is_a_keeper(seen, new_ref)) {
						/* we found an easier way to make this value */
						seen.insert(new_ref->str());
						new_refs.push_back(new_ref);
					}
				} catch (const std::exception &) {
				}
			}
			return new_refs;
		},
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the times operation, which needs two directions */
			for (auto other : others) {
				try {
					node::ref new_ref = times::create(key, other);
					if (is_a_keeper(seen, new_ref)) {
						/* we found an easier way to make this value */
						seen.insert(new_ref->str());
						new_refs.push_back(new_ref);
					}
				} catch (const std::exception &) {
				}
			}
			return new_refs;
		},
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the negate operation */
			try {
				node::ref new_ref = negate::create(key);
				if (is_a_keeper(seen, new_ref)) {
					/* we found an easier way to make this value */
					seen.insert(new_ref->str());
					new_refs.push_back(new_ref);
				}
			} catch (const std::exception &) {
			}
			return new_refs;
		},
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the sqroot operation */
			try {
				node::ref new_ref = sqroot::create(key);
				if (is_a_keeper(seen, new_ref)) {
					/* we found an easier way to make this value */
					seen.insert(new_ref->str());
					new_refs.push_back(new_ref);
				}
			} catch (const std::exception &) {
			}
			return new_refs;
		},
		[&seen](node::ref key, node::refs others) -> node::refs {
			node::refs new_refs;
			/* handle the factorial operation */
			try {
				node::ref new_ref = factorial::create(key);
				if (is_a_keeper(seen, new_ref)) {
					/* we found an easier way to make this value */
					seen.insert(new_ref->str());
					new_refs.push_back(new_ref);
				}
			} catch (const std::exception &) {
			}
			return new_refs;
		},
	};

	const int max_value = atoi(argv[1]) + 1;
	const int min_value = 0;
	std::map<int, node::ref> founds;
	trees.push_back(base);
	while (true) {
		node::refs new_trees;
		for (auto tree : trees) {
			for (auto permute : permutations) {
				auto newest_trees = permute(tree, trees);
				std::copy(newest_trees.begin(), newest_trees.end(), std::back_inserter(new_trees));
				for (auto tree : newest_trees) {
					int count = tree->count_fours();
					if (count == 4) {
						int value = tree->get_value();
						if (value >= min_value && value < max_value) {
							if (founds.find(value) == founds.end()) {
								founds[value] = tree;
								// printf("% 4d = %s\n", tree->get_value(), tree->str().c_str());
							}
						}
					}
				}
				if (founds.size() >= max_value) {
					break;
				}
			}
			if (founds.size() >= max_value) {
				break;
			}
		}
		if (founds.size() >= max_value) {
			break;
		}
		if (new_trees.size() == 0) {
			break;
		} else {
			// printf("%d %d\n", (int)new_trees.size(), (int)founds.size());
		}
		std::copy(new_trees.begin(), new_trees.end(), std::back_inserter(trees));
	}
	std::map<int, node::ref> list;
	for (auto tree : founds) {
		list.insert({tree.first, tree.second});
	}
	for (auto tree : list) {
		printf("% 3d = %s\n", tree.first, tree.second->str().c_str());
	}
}
