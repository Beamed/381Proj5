#ifndef AGENT_FACTORY_H
#define AGENT_FACTORY_H

#include <string>
#include <memory>

struct Point;
class Agent;

// Create and return the shared_ptr to the specified Agent type. If the type
// is unrecognized, throws Error("Trying to create agent of unknown type!")
std::shared_ptr<Agent> create_agent(const std::string& name, const std::string& type, Point location);

#endif
