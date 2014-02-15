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
      friend class Router;
      public:
        Node() { };
        Node(std::string p, Node* const& pr);
        ~Node();

        Node* unify(Node* const& root, std::string const& path, params_map& params);
        Node* unify(Node* const& root, Node* const path, params_map& params);
        bool merge(Node* const path);
        static Node* from_path(std::string const& path);

        void inject(Node* const& rhs, params_map& params);

        void add_service(std::shared_ptr<LambdaService> srv) {
          service.clear();
          service.resize(Router::WORKERS);

          for (int i = 0; i < Router::WORKERS; i++)
            service[i] = std::make_shared<LambdaService>(srv);
        }

        template <class T>
        void add_service(std::shared_ptr<T> srv) {
          service.clear();
          service.resize(Router::WORKERS);

          for (int i = 0; i < Router::WORKERS; i++)
            service[i] = std::make_shared<T>();
        };
        std::shared_ptr<Service> find_service(int worker_id);

        bool is_root();
        bool is_last();
        bool is_splat();
        Node* next();
        Node* start();
        Node* end();

        std::string uri();

        void print(int level);

        static struct Less {
          bool operator()(const Node* a, const Node* b) const {
            if (a->path[0] == '*')
              return false;
            if (b->path[0] == '*')
              return true;

            if (a->path[0] == ':')
              if (b->path[0] != ':')
                return false;
            if (b->path[0] == ':')
              if (a->path[0] != ':')
                return true;

            return a->path < b->path;
          }
        } less;

        static struct Unifiable {
          bool operator()(const Node* a, const Node* b) const {
            if (a == nullptr || b == nullptr)
              return false;

            if (b->path.size() > 0 && (b->path[0] == '*' || b->path[0] == ':'))
              return false;
            if (a->path.size() > 0 && (a->path[0] == '*' || a->path[0] == ':'))
              return true;

            return a->path == b->path;
          }
        } unifiable;

        static struct Equal {
          bool operator()(const Node* a, const Node* b) const {
            return (!less(a,b)) && (!less(b,a));
          }
        } equal;

      protected:
        std::string path;
        Node* parent = nullptr;
        std::set<Node*, Less> children;

        std::vector< std::shared_ptr<Service> > service;
    };

  public:
    static int WORKERS;
    static Router* Instance();

    static std::shared_ptr<Service> getResource(std::shared_ptr<Request>, int);
    static void path(std::string const &, service_lambda);

    static Node* match(std::string const&, params_map&);

    template <class R>
    void resource(std::string const& path) {
      Router::Node* node = Router::Node::from_path(path);
      node->end()->add_service(std::make_shared<R>());
      root->merge(node);

      Router::Node* splat_node = Router::Node::from_path(path + "/*");
      splat_node->end()->service = node->end()->service;
      root->merge(splat_node);
    }

    ~Router();

  private:
    Router();
    static Router* pInstance;

    static Node* root;
};

}

#endif
