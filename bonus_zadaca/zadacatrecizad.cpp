#include<iostream>
#include<list>
#include<algorithm>
using namespace std;

class Object
{
  int pos;
public:
  int position()const
  {
      return pos;
  }
  int & position()
  {
      return pos;
  }

};

class Node
{
  static const unsigned int depth_max = 32;
  static const unsigned int max_objects = 32;

  const unsigned int depth;
  const int min, max, center;
  std::list<Object*> objects;
  Node *children[2];

  Node(Node const &, bool);
  bool isEmpty() const;
public:
  Node(int min, int max);
  ~Node();
  Node(Node const&, Node const&);
  void addObject(int position, Object *);
  void addObject2(int position, Object *);
  void delObject(Object *);
  void delObject2(Object *);
  void movObject(int newPos, Object *);

  void getObject(int posMin, int posMax, std::list<Object*> &);
};

Node::Node(int min, int max)
        : depth(0), min(min), max(max), center((min + max) / 2), objects()
{for (int i=0; i<2; i++)
    {
        children[i] = NULL;
    }}

Node::~Node()
{
    for (int i = 0; i < 2; i++)
    {
        delete children[i];
    }
}

Node::Node(Node const&father, Node const&c):
 depth(father.depth + 1),
            min(c.min ? father.min : father.center),
            max(c.max ? father.center : father.max),
            center((min + max) / 2),
            objects()
{for (int i=0; i<2; i++)
    {
        children[i] = NULL;
    }}

Node::Node(Node const & father, bool side)
        : depth(father.depth + 1),
          min(side ? father.min : father.center),
          max(side ? father.center : father.max),
          center((min + max) / 2),
          objects()
{for (int i=0; i<2; i++)
    {
        children[i] = NULL;
    }}

bool Node::isEmpty() const
{
    return !((!objects.empty()) || children ||
             (children[0]->isEmpty() && children[1]->isEmpty()));
}

void Node::addObject(int position, Object *obj)
{
    if (objects.size() > max_objects && depth < depth_max)
    {
        if (!children) {
            children[0] = new Node(0, 0);
            children[1] = new Node(0, 0);
        }
        return;
    }
    objects.push_back(obj);
}

void Node::delObject(Object * obj)
{

    if (children && children[0]->isEmpty() && children[1]->isEmpty()){
        for (int i = 0; i < 2; i++)
        {
            delete children[i];
        } }
}

void Node::addObject2(int position, Object * obj)
{
    if (objects.size() > max_objects)
        if (depth < depth_max)
        {
            if (children == nullptr)

                children[0] = new Node{ Node(*this, false), Node(*this, true) };
                children[1] = new Node{ Node(*this, false), Node(*this, true) };

            children[ position <= center ? 0 : 1]->addObject(position, obj);
            return;
        }
    objects.push_back(obj);
    obj->position() = position;
}

void Node::delObject2(Object * obj)
{
    auto found = std::find(objects.begin(), objects.end(), obj);
    if (found != objects.end())
    {
        objects.remove(*found);
        return;
    }
    if (children)
    {

        children[obj->position() <= center ? 0 : 1]->delObject(obj);
        if (children[0]->isEmpty() && children[1]->isEmpty())
            for (int i = 0; i < 2; i++)
            {
                delete children[i];
            }
    }
}
void Node::movObject(int newPos, Object *obj)
{
    auto found = std::find(objects.begin(), objects.end(), obj);
    if (found == objects.end() && children)
    {

        if (newPos <= center && obj->position() <= center)
            children[0]->movObject(newPos, obj);
        else if (newPos > center && obj->position() > center)
            children[1]->movObject(newPos, obj);


        else if (newPos <= center && obj->position() > center)
        {
            children[1]->delObject(obj);
            children[0]->addObject(newPos, obj);
        }
        else if (newPos > center && obj->position() <= center)
        {
            children[0]->delObject(obj);
            children[1]->addObject(newPos, obj);
        }
    }
    obj->position() = newPos;
}

void Node::getObject(int posMin, int posMax, std::list<Object *> & list)
{
    for (auto it = objects.begin(); it != objects.end(); it++)
        if ((*(it))->position() >= posMin && (*(it))->position() <= posMax)
            list.push_back(*it);

    if (children[0] and children[1])
    {
        if (posMin <= center)
            children[0]->getObject(posMin, posMax, list);
        if (posMax > center)
            children[1]->getObject(posMin, posMax, list);
    }
}

int main()
{
    return 0;
}