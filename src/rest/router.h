#ifndef REST_CPP_ROUTER_H
#define REST_CPP_ROUTER_H

#include <set>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
#include <vector>
#include "service.h"
#include "lambda_service.h"

namespace REST {

typedef std::map<std::string, std::shared_ptr<Service> > names_services_map;
typedef std::vector< std::shared_ptr<names_services_map> > workers_services_vector;
typedef std::map<std::string, std::string> params_map;
typedef std::pair<std::string, std::shared_ptr<params_map> > path_tuple;
typedef std::function<void(Service *)> service_lambda;
typedef std::pair<std::string, std::shared_ptr<LambdaService> > path_lambda;
typedef std::map<std::string, path_lambda> lambda_patterns;

/**
 * Router resolves URL to Request::parameters and
 * finds Service corresponding to the URL.
 *
 * @see Request
 * @see Service
 */
class Router {
  private:
    class Node {
      friend class Node;
      public:
        Node() { };
        Node(std::string p);
        virtual ~Node();

        bool unify(std::string const& path, params_map& params);
        bool unify(Node* const path, params_map& params);
        bool merge(Node* const path);
        static Node* from_path(std::string const& path);

        bool is_last();
        const Node* next();

        std::string uri();

        void print(int level);

        static struct Less {
          bool operator()(const Node* a, const Node* b) const {
            if (a->path == "*")
              return false;
            if (b->path == "*")
              return true;

            if (a->path[0] == ':')
              if (b->path[0] != ':')
                return false;
            if (b->path[0] == ':')
              if (a->path[0] != ':')
                return true;

            std::cout << "lol\n";
            return a->path < b->path;
          }
        } less;

        static struct Equal {
          bool operator()(const Node* a, const Node* b) const {
            std::cout << "lol2\n";
            return (!less(a,b)) && (!less(b,a));
          }
        } equal;

      protected:
        std::string path;
        std::set<Node*, Less> children;
    };

    class RootNode final :  public Node {
      public:
        RootNode();
/*
        virtual bool operator<(Node* const& other) const {
          return true;
        }
  */  };

    class ParameterNode : public Node {
      public:
        ParameterNode(std::string const& name);
/*
        virtual bool operator<(Node* const& other) const {
          return false;
        }

        virtual bool operator<(ParameterNode* const& other) const {
          return path < other->path;
        }
*/
      protected:
        std::string name;
    };

    class SplatNode : public ParameterNode {
      public:
        SplatNode();
    };

  public:
    LambdaService ls;
    static int WORKERS;
    static Router* Instance();

    static std::shared_ptr<Service> getResource(std::shared_ptr<Request>, int);
    static void path(std::string const &, service_lambda);

    void route(std::string);

    template <class R>
    void resource(std::string const& path) {
      ServiceRegister<R> reg(path);
    }

  private:
    Router();
    static Router* pInstance;
    static workers_services_vector workers_services;
    static path_tuple* extractParams(std::string const&);
    static lambda_patterns* patterns;

    RootNode* root;
};

}

#endif
