#include "Days.h"
#include "AdventUtils.h"
#include <cassert>
#include <vector>

using namespace std;

struct SnailfishNode
{
	SnailfishNode(int _value)
		: left(nullptr), right(nullptr), value(_value)
	{
	}

	SnailfishNode()
		: SnailfishNode(-1)
	{
	}

	SnailfishNode(const SnailfishNode& node)
	{
		left = node.left ? new SnailfishNode(*node.left) : nullptr;
		right = node.right ? new SnailfishNode(*node.right) : nullptr;
		value = node.value;
	}
	
	bool IsLeaf() const
	{
		return left == nullptr && right == nullptr && value >= 0;
	}

	bool Contains(const SnailfishNode* node) const
	{
		return this == node || (left && left->Contains(node)) || (right && right->Contains(node));
	}

	int GetMagnitude() const
	{
		int magnitude = 0;
		magnitude += (left->IsLeaf() ? left->value : left->GetMagnitude()) * 3;
		magnitude += (right->IsLeaf() ? right->value : right->GetMagnitude()) * 2;
		return magnitude;
	}

	friend ostream& operator<<(ostream& os, const SnailfishNode& node)
    {
        if (node.IsLeaf()) 
        {
            os << node.value;
        }
        else
        {
            os << '[';        
            os << *node.left;
            os << ',';
            os << *node.right;
            os << ']';        
        }
		return os;
    }
	
	static SnailfishNode* Construct(const string& expression)
	{
		size_t idx = 0;
		return Construct(expression, idx);
	}

	static SnailfishNode* Construct(const string& expression, size_t& idx)
	{
		SnailfishNode* node = new SnailfishNode();
		char c = expression[idx];
		if (c == '[')
		{
			node->left = Construct(expression, ++idx);
			++idx; // ,
			node->right = Construct(expression, ++idx);
			++idx; // ]
		}
		else
		{
			node->value = CharToInt(c);
		}
		return node;
	}

	static void Destruct(const SnailfishNode* node)
	{
		if (node)
		{
			Destruct(node->left);
			Destruct(node->right);
			delete node;
		}
	}
	
	int value;
	SnailfishNode* left;
	SnailfishNode* right;
};

struct ExplodeResult
{
	ExplodeResult()
		: explodeNode(nullptr), explodeValues{-1, -1}
	{
	}
	
	void AssignFromNode(const SnailfishNode& _explodeNode)
	{
		assert(_explodeNode.left && _explodeNode.left->IsLeaf());
		assert(_explodeNode.right && _explodeNode.right->IsLeaf());
		explodeNode = &_explodeNode;
		explodeValues = {_explodeNode.left->value, _explodeNode.right->value};
	}
	
	void TryAddExplodeResult(SnailfishNode* node)
	{
		if (node)
		{
			TryAddExplodeResult(node->left, explodeValues.first);
			TryAddExplodeResult(node->right, explodeValues.second);
		}
	}

private:
	void TryAddExplodeResult(SnailfishNode* node, int& result) const
	{
		if (result >= 0 && node && !node->Contains(explodeNode))
		{
			if (node->IsLeaf())
			{
				// If result was successfully added, nullify it to prevent multiple adds
				node->value += result;
				result = -1;
			}
			else if (&result == &explodeValues.first)
			{
				TryAddExplodeResult(node->right, result);
				TryAddExplodeResult(node->left, result);
			}
			else
			{
				TryAddExplodeResult(node->left, result);
				TryAddExplodeResult(node->right, result);
			}
		}
	}
	
	pair<int, int> explodeValues = {-1, -1};
	const SnailfishNode* explodeNode = nullptr;
};

bool Explode(SnailfishNode* node, size_t depth, ExplodeResult& explodeResult)
{
	if (node && !node->IsLeaf())
	{
		if (depth == 4)
		{
			// Depth of 4 should always have only leaf nodes
			explodeResult.AssignFromNode(*node);
			node->value = 0;
			SnailfishNode::Destruct(node->left);
			SnailfishNode::Destruct(node->right);
			node->left = nullptr;
			node->right = nullptr;
			return true;
		}
		else
		{
			++depth;
			if (Explode(node->left, depth, explodeResult) || Explode(node->right, depth, explodeResult))
			{
				explodeResult.TryAddExplodeResult(node);
				return true;
			}
		}
	}
	return false;
}

bool Explode(SnailfishNode* node)
{
	ExplodeResult explodeResult;
	return Explode(node, 0, explodeResult);
}

bool Split(SnailfishNode* node)
{
	if (node)
	{
		if (node->value >= 10)
		{
			node->left = new SnailfishNode(node->value / 2);
			node->right = new SnailfishNode((node->value + 1) / 2);
			node->value = -1;
			return true;
		}
		if (Split(node->left))
			return true;
		if (Split(node->right))
			return true;
	}
	return false;
}

void Reduce(SnailfishNode* node, ostream& os)
{
	bool keepReducing = false;
	do
	{
		keepReducing = false;
		if (Explode(node))
		{
			keepReducing = true;
			//os << "Exp: " << *node << endl;
		}
		else if (Split(node))
		{
			keepReducing = true;
			//os << "Spl: " << *node << endl;
		}
	}
	while (keepReducing);
}

const SnailfishNode* Add(const SnailfishNode& left, const SnailfishNode& right, ostream& os)
{
	SnailfishNode* node = new SnailfishNode();
	node->left = new SnailfishNode(left);
	node->right = new SnailfishNode(right);
	//os << "Add: " << *node << endl;
	Reduce(node, os);
	return node;
}

template <>
void Run<Day18>(Part part, istream& is, ostream& os)
{
	vector<const SnailfishNode*> nodes = {};
	
	string arg;
	while (getline(is, arg))
	{
		nodes.push_back(SnailfishNode::Construct(arg));
	}
	if(part == Part01)
	{
		const SnailfishNode* baseNode = nullptr;
		for (const SnailfishNode* node : nodes)
		{
			baseNode = baseNode ? Add(*baseNode, *node, os) : node;
		}
		if (baseNode)
		{
			os << "Magnitude for sum of " << nodes.size() << " nodes: " <<  baseNode->GetMagnitude() << endl;
			SnailfishNode::Destruct(baseNode);
		}
	}
	else if(part == Part02)
	{
		int magnitudeMax = 0;
		const SnailfishNode* node1Max = nullptr;
		const SnailfishNode* node2Max = nullptr;
		for (const SnailfishNode* node1 : nodes)
		{
			for (const SnailfishNode* node2 : nodes)
			{
				if (node1 != node2)
				{
					const SnailfishNode* nodeSum = Add(*node1, *node2, os);
					int magnitude = nodeSum->GetMagnitude();
					if (magnitude > magnitudeMax)
					{
						magnitudeMax = magnitude;
						node1Max = node1;
						node2Max = node2;
					}
					SnailfishNode::Destruct(nodeSum);
				}
			}
		}
		if (node1Max && node2Max)
		{
			os << "Magnitude Max: " << magnitudeMax << endl;
			os << "Node1: " << *node1Max << endl;
			os << "Node2: " << *node2Max << endl;
		}
	}
	for (const SnailfishNode* node : nodes)
	{
		SnailfishNode::Destruct(node);
	}
	nodes.clear();
}