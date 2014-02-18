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
        void add_service(const T* srv) {
          service.clear();
          service.resize(Router::WORKERS);
          std::cout << "dodaje: " << typeid(T).name() << std::endl;
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
      node->end()->add_service((R*)(NULL));
      root->merge(node);

      Router::Node* splat_node = Router::Node::from_path(path == "/" ? "/*" : (path+"/*"));
      splat_node->end()->service = node->end()->service;
      root->merge(splat_node);
    }

    template <class R, size_t N>
    void resource() {
      std::string name = typeid(R).name();
      std::string path = "";
      path.reserve(name.size());
      std::transform(name.begin(), name.end(), name.begin(), ::tolower);
      size_t number_position = 0;
      size_t strip = N;
      while ((number_position = name.find_first_of("0123456789")) != std::string::npos) {
        if (number_position != 0)
          name = name.substr(number_position);
        std::string number;

        for (size_t next_digit = 0; ::isdigit(name[next_digit]); next_digit++)
          number += name[next_digit];

        size_t name_length = std::stol(number);

        name = name.substr(number.size());
        if (strip == 0)
          path += "/"+name.substr(0, name_length);
        else
          strip--;
        name = name.substr(name_length);
      }

      auto s = path.rfind("service");
      if (s == (path.size() - 7))
        path.erase(path.rfind("service"));

      s = path.rfind("resource");
      if (s == (path.size() - 8))
        path.erase(path.rfind("resource"));

      resource<R>(path);
    }

    template <class R>
    void resource() {
      resource<R, 0>();
    }

    void print();

    ~Router();

  private:
    Router();
    static Router* pInstance;

    static Node* root;
};

}

#endif
