#include "Collection.h"

/*
Вставили рут. Если новый элемент полностью включает в себя рут, то заменяем этот элемент на рут, делаем ребёнком рут. Если не полностью включает, 

Если совсем не включает, всё равно обьединяем.

*/


void Node::Insert(Set& input_set) {


	Set work_set;
	int min_difference = input_set.data.size();
	int reserve_min = min_difference;
	int index_min_diff = -1;

	Node k = *this;
	// Если sets пустой
	if (sets_pointers[0].first.data.size() == 0)
	{
		sets_pointers[0].first = input_set;
		sets_pointers[0].first.is_user_set = 1;
		return;
	}

	// Находим минимальную разницу между Входящим сэтом и любым сэтом из вектора сэтов.
	std::vector<std::pair<Set,Node*>>::iterator iter = sets_pointers.begin();
	bool max_minus_found = false;

	while (iter != sets_pointers.end())
	{
		if ((min_difference > input_set.Minus(iter->first).data.size()) && !(max_minus_found) && !((iter->first).data.empty()))
		{
			index_min_diff = std::distance(sets_pointers.begin(), iter);
			min_difference = input_set.Minus(iter->first).data.size();
			work_set = iter->first;
			if (min_difference == 0)
			{
				max_minus_found = true;
			}
		}
		++iter;

	}
	// Проверка на пустые места в ноде
	std::pair<int, int> emp_space_in_root = HaveEmptySpace(sets_pointers);
	// Если наш сэт, не пересикается ни с одним из сэтов
	if (min_difference == reserve_min){
		// Cоздаём cэт, что обьединяет все сэти в ноде, вместе с входящим 
		Set new_set = UnionNodeSets(sets_pointers).Union(input_set);

		// Ищем есть ли пустые места в ноде, если нету, (еmp_space.first == -1) => Создаем новый нод, который будет родителем этого нода
		// В него включаем наш новый сэт (обьединения)
		
		// Если есть свободное место, просто вставляем в это свободное место сэт, если предок является надмножество. А иначе, нужно найти пустое место
		// у предка, если у предка нету пустого места, заменяем предка, на обьединение с предком. И вставляем этот нод как ребенок этого обьеденения.
		if (emp_space_in_root.first != -1)
		{
			sets_pointers[emp_space_in_root.second].first = input_set;
			return;
		}

		// Если нету свободного места
		else
		{
			// Создали новый нод, сэтом которого будет обьединение всх сэтов текущего нода
			Node* new_node = new Node();
			new_node->sets_pointers[0].first = new_set;
			if (parent != nullptr)
			{
				// Находим индекс, каким по-порядку был текущий нод, в векторе указателей родителя. 
				int index = IndexOfChildVector(parent->sets_pointers, this);
				// Установили на этот индекс новый нод
				parent->sets_pointers[index].second = new_node;
				// Первый указатель этого нового нода - это текущий нод.
				new_node->sets_pointers[0].second = this;
				// Установили вторым сэтом, в новом ноде, входной сэт.
				new_node->sets_pointers[1].first = input_set;
				return;
			}
		}
	}
	// Если минимальный сэт, является подмножеством входящего сэта
	if (sets_pointers[index_min_diff].first.IsSubsetOf(input_set) && !(sets_pointers[index_min_diff].first.data.empty())) {
		Node* new_node = new Node();
		sets_pointers[index_min_diff].first = input_set;
		new_node->sets_pointers[0].second = sets_pointers[index_min_diff].second;
		sets_pointers[index_min_diff].second = new_node;
		new_node->sets_pointers[0].first = work_set;
		return;
	}
	// Eсли минимальный сэт не является подмножеством входящего
	else
	{
		if (emp_space_in_root.first != -1) {
			Node* new_node = new Node();
			sets_pointers[emp_space_in_root.second].first = input_set;
			// Если входящее множество надмножество для каждого элемента
			// ребенка минимального элемента -> назначаем этого ребенка, ребенком нашего входящего сэта
 		
			if (sets_pointers[index_min_diff].second != nullptr)
			{
				bool change_pointer = true;
				std::vector<std::pair<Set, Node*>>::iterator iter = sets_pointers[index_min_diff].second->sets_pointers.begin();
				while (iter != sets_pointers[index_min_diff].second->sets_pointers.end())
				{
					if (!iter->first.IsSubsetOf(input_set))
					{
						change_pointer = false;
						iter = sets_pointers[index_min_diff].second->sets_pointers.end() - 1;
						break;
					}
					iter++;
				}
				if (change_pointer)
				{
					sets_pointers[emp_space_in_root.second].second = this->sets_pointers[index_min_diff].second;
				}

			}
			
			return;
		}
		Set new_set = UnionNodeSets(sets_pointers);
		Node* new_node = new Node();
		new_node->sets_pointers[0].first = new_set;
		new_node->sets_pointers[0].second = this;
		new_node->sets_pointers[1].first = input_set;
		return;
		/*std::pair<int,int> emp_spc = HaveEmptySpace(sets_pointers[index_min_diff].second->sets_pointers);*/
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
	while (iter->first.data.capacity() != 0 && iter != vector_to_union.end()) {
		union_set = union_set.Union(iter->first);
		if (iter != vector_to_union.end())
		{
			iter++;

		}
	}
	return union_set;
}

// Количество пустих мест, вместе с индексом первого пустого места в векторе сэтов. Первое -> количество пустых мест. Второе->индекс.
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
		if (iter != vector_to_check.end())
		{
			iter++;

		}
	}
	return { empty_space,index };
}

// Если индекс -1 => Нету такого ребенка
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
		if (iter != node_vec.end())
		{
			iter++;

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
		if (iter != set_vec.end())
		{
			iter++;

		}
	}
	
	return { min_size, final_ind };
}




//if (parent == nullptr)
//{
	// Если нету родителя, создаём нового, добавляем ему в сэты, нулевым индексом, новый сэт, 
	// нулевым указателем, текущий нод, добавляем в текущий нод входной сэт
//	parent = new Node();
//	parent->sets_pointers[0].first = new_set;
//	parent->sets_pointers[0].second = this;
//	this->sets_pointers[emp_space.second].first = input_set;
//	return;
//}
//else
//{
//	Node new_node = Node();
//	new_node.sets_pointers[0].first = new_set;
//	int index = IndexOfChildVector(parent->sets_pointers, this);
//	parent->sets_pointers[index].second = &new_node;
//	new_node.sets_pointers[0].second = this;
//	this->sets_pointers[emp_space.second].first = input_set;
//	return;
//}


	/*else {

		this->sets_pointers[index_min_diff].second->Insert(input_set);
	}*/

	// 	   //не имеет ребёнка//
	//else if(sets_pointers[index_min_diff].second == nullptr)
	//{
	//	Node* new_node = new Node();
	//	new_node->sets_pointers[0].first = input_set;
	//	new_node->parent = this;
	//	sets_pointers[index_min_diff].second = new_node;
	//	return;
	//}