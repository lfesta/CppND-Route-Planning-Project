#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

	start_node = &m_Model.FindClosestNode(start_x, start_y);

  	end_node = &m_Model.FindClosestNode(end_x, end_y);
  
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {  

  	return node->distance(*end_node);

}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
  	if (current_node == start_node) {
    	current_node->visited = true;
    }
  
 	current_node->FindNeighbors();
  
	for (RouteModel::Node * node: current_node->neighbors) {
        node->parent = current_node;
        node->h_value = this->CalculateHValue(node);
        node->g_value = current_node->g_value + current_node->distance(*node);
        node->visited = true;
        open_list.push_back(node);
    }
      
}

RouteModel::Node *RoutePlanner::NextNode() {

  	std::sort(open_list.begin(), open_list.end(), [](auto const &a, auto const &b) {return (a->g_value + a->h_value) > (b->g_value + b->h_value) ;});

  	auto smallest_element = open_list.back();
  
  	open_list.pop_back();
  
  	return smallest_element;
  
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
	
    // TODO: Implement your solution here.
  	while (current_node != start_node) {
    	distance += current_node->distance(*current_node->parent);
      	path_found.push_back(*current_node);
      	current_node = current_node->parent;
    }
  	
  	// Last push_back to include starting node
  	path_found.push_back(*current_node);
  
  	std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

void RoutePlanner::AStarSearch() {
  
    RouteModel::Node *current_node = nullptr;
  
    // TODO: Implement your solution here.
  
  	this->AddNeighbors(start_node);
  	
    while (open_list.size()>0) {
      	current_node = this->NextNode();
      	if (current_node == end_node) {
      		break;
      		}
      
    	this->AddNeighbors(current_node);
      
    }
  
  	m_Model.path = this->ConstructFinalPath(current_node);
   
}