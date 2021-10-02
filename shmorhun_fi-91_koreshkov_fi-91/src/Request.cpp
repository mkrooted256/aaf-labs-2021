#include "Collection.h"


void Node::Insert(Set& input_set) {


	Set work_set;
	int min_difference = input_set.data.size();
	int reserve_min = min_difference;

	// Если sets пустой
	if (sets_pointers[0].first.data.size() == 0)
	{
		sets_pointers[0].first = input_set;
		sets_pointers[0].first.is_user_set = 1;
		return;
	}

	/* Варианты: min_difference == reserve_min*/

	// Находим минимальную разницу между Входящим сэтом и любым сэтом из вектора сэтов.
	std::vector<std::pair<Set,Node*>>::iterator iter = sets_pointers.begin();
	while (iter->first.data.size() != 0 && iter != sets_pointers.end())
	{
		if (min_difference > input_set.Minus(iter->first).data.size());
		{
			min_difference = input_set.Minus(iter->first).data.size();
			work_set = iter->first;
		}

	}

	if (min_difference == reserve_min){
		Set new_set = UnionNodeSets(sets_pointers).Union(input_set);
		std::pair<int,int> emp_space = HaveEmptySpace(sets_pointers);
		if (emp_space.first != -1)
		{
			
			if (parent == nullptr)
			{
				// Если нету родителя, создаём нового, добавляем ему в сэты, нулевым индексом, новый сэт, 
				// нулевым указателем, текущий нод, добавляем в текущий нод входной сэт
				parent = new Node();
				parent->sets_pointers[0].first = new_set;
				parent->sets_pointers[0].second = this;
				this->sets_pointers[emp_space.second].first = input_set;
				return;
			}
			else
			{
				Node new_node = Node();
				new_node.sets_pointers[0].first = new_set;
			    int index = IndexOfChildVector(parent->sets_pointers, this);
				parent->sets_pointers[index].second = &new_node;
				new_node.sets_pointers[0].second = this;
				this->sets_pointers[emp_space.second].first = input_set;
				return;
			}

		}
		else
		{
			// Создали новый нод, сэтом которого будет обьединение всх сэтов текущего нода
			Node new_node = Node();
			new_node.sets_pointers[0].first = new_set;
			// Находим индекс, каким по-порядку был текущий нод, в векторе указателей родителя. 
			int index = IndexOfChildVector(parent->sets_pointers, this);
			// Установили на этот индекс новый нод
			parent->sets_pointers[index].second = &new_node;
			// Первый указатель этого нового нода - это текущий нод.
			new_node.sets_pointers[0].second = this;
			// Установили вторым сэтом, в новом ноде, входной сэт.
			new_node.sets_pointers[1].first = input_set;
		}
	}
	/*нашли новый max_similarity*/

}

// Первое - индекс 
//std::pair<int,int> Node::ShortestSubTree(std::vector<std::pair<Set, Node*>>) {
//
//}

Set Node::UnionNodeSets(std::vector<std::pair<Set, Node*>>& vector_to_union) {
	Set union_set;
	std::vector<std::pair<Set, Node*>>::iterator iter = vector_to_union.begin();
	while (iter->first.data.capacity() != 0) {
		union_set = union_set.Union(iter->first);
	}
	return union_set;
}

// Количество пустих мест, и индекс первого пустого места в векторе сэтов. Первое -> количество пустых мест. Второе->индекс.
std::pair<int,int> Node::HaveEmptySpace(std::vector<std::pair<Set,Node*>> vector_to_check) {
	int empty_space = 3;
	int index = -1;
	std::vector<std::pair<Set, Node*>>::iterator iter = vector_to_check.begin();
	while (iter != vector_to_check.end())
	{
		index++;
		
		if (iter->first.data.size() == 0)
		{
			return { empty_space, index };
		}
		empty_space--;
	}
	return { empty_space,index };
}

int Node::IndexOfChildVector(std::vector<std::pair<Set,Node*>>& node_vec, Node* node) {
	int index = -1;
	std::vector <std::pair<Set, Node*>> ::iterator iter = node_vec.begin();
	while (iter != node_vec.end())
	{
		index++;
		if (iter->second == node)
		{
			return index;
		}
	}
	return index;
}

// Первое - минимальный размер сэта, второе - индекс этого сэта.
std::pair<int, int> Node::GetMinimumSetSizeAndIndex(std::vector<std::pair<Set, Node*>>& set_vec) {
	int min_size = -1;
	int index = -1;
	int final_ind = -1;
	std::vector<std::pair<Set,Node*>>::iterator iter = set_vec.begin();
	while (iter != set_vec.end())
	{
		index++;
		if (min_size > iter->first.data.size()){
			final_ind = index;
			min_size = iter->first.data.size();
		}
	}
	
	return { min_size, final_ind };
}