#ifndef LEXICAL_GRAPH_GRAPH_H_
#define LEXICAL_GRAPH_GRAPH_H_

#include <vector>
#include <memory>
#include <ostream>
#include "State.h"

namespace ecc {

class Graph {
public:

	/**
	 * Get root state
	 */
	std::shared_ptr<State> getRoot() const { return root; }

	/**
	 * Get state and return its id
	 */
	int addState();

	/**
	 * Add transition between two states
	 */
	void addTransition(int fromState, int toState, std::string label);

	/**
	 * Get state by id
	 * @return pointer to the state
	 */
	std::shared_ptr<State> getStateById(int id) const {return this->states[id];}

	/**
	 * Check if graph has an initial state
	 * @return true|false
	 */
	bool hasInitialState() const {return this->initialState;}

	void setInitialState(bool initialState) {this->initialState = initialState;}
private:

	// List of all states
	std::vector<std::shared_ptr<State>> states;

	// Starting state of the graph
	std::shared_ptr<State> root;

	// Store if initial state is defined
	bool initialState = false;

	/**
	 * To string
	 */
	friend std::ostream& operator<<(std::ostream& os, const Graph &graph) {
	    os << "Graph has " << graph.states.size() << " states";
		for(size_t i=0; i < graph.states.size(); i++){
	    	os << "\n\t" << *graph.states[i];
	    }
	    return os;
	}
};
}

#endif