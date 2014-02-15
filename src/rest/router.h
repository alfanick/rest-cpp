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
    class Node : public std::enable_shared_from_this<Node> {
      friend class Node;
      public:
        Node() { };
        Node(std::string p, std::shared_ptr<Node> const& pr);
        ~Node();

        bool unify(std::shared_ptr<Node> const& root, std::string const& path, params_map& params);
        std::shared_ptr<Node> unify(std::shared_ptr<Node> const& root, std::shared_ptr<Node> const path, params_map& params);
        bool merge(std::shared_ptr<Node> const path);
        static std::shared_ptr<Node> from_path(std::string const& path);

        void inject(std::shared_ptr<Node> const& rhs, params_map& params);

        bool is_root();
        bool is_last();
        bool is_splat();
        std::shared_ptr<Node> next();

        std::string uri();

        void print(int level);

        static struct Less {
          bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const {
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
          bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const {
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
          bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const {
            return (!less(a,b)) && (!less(b,a));
          }
        } equal;

      protected:
        std::string path;
        std::shared_ptr<Node> parent = nullptr;
        std::set<std::shared_ptr<Node>, Less> children;
    };

  public:
    LambdaService ls;
    static int WORKERS;
    static Router* Instance();

    static std::shared_ptr<Service> getResource(std::shared_ptr<Request>, int);
    static void path(std::string const &, service_lambda);

    void route(std::string);
    void match(std::string const&, params_map&);

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

    std::shared_ptr<Node> root;
};

}

#endif
