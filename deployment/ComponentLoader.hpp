#ifndef ORO_COMPONENTLOADER_HPP_
#define ORO_COMPONENTLOADER_HPP_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "../rtt-fwd.hpp"

namespace OCL {
        /**
         * Locates Component libraries found on the filesystem and keeps track of loaded Components.
         * In case a no components of a component library are running, the library can be unloaded.
         *
         * @name Component Paths
         * Paths are scanned in this order:
         *
         * ** First the paths specified by the function argument \a path_list if the function takes such argument
         * ** Second the paths specified using the setComponentPath() function.
         *
         * If neither is specified, it looks for Components in the current directory (".").
         */
        class ComponentLoader
        {
            static boost::shared_ptr<ComponentLoader> minstance;

            /**
             * Keep a list of all loaded libraries such that double
             * loads are avoided during import/loadLibrary.
             */
            class LoadedLib{
            public:
                LoadedLib(std::string n, std::string short_name, void* h)
                : filename(n), shortname(short_name), handle(h)
                {
                }
                /**
                 * File name of the library.
                 */
                std::string filename;
                /**
                 * Short name of the library (without lib/dll/so)
                 */
                std::string shortname;

                /**
                 * Library handle.
                 */
                void* handle;

                std::vector<std::string> components_type;
            };

            std::vector< LoadedLib > loadedLibs;

            /**
             * Path to look for if all else fails.
             */
            std::string Component_path;

            /**
             * Internal function that does all library loading.
             * @param filename The path+filename to open
             * @param shortname The short name of this file
             * @param log_error Log errors to users. Set to false in case you are poking
             * files to see if they can be loaded.
             * @return true if a new library was loaded or if this library was already loaded.
             */
            bool loadInProcess(std::string filename, std::string shortname, bool log_error );
            /**
             * Helper function for loadComponent.
             * @param name
             * @param path_list
             * @param subdir
             * @param kind
             * @return
             */
            bool loadComponentInternal( std::string const& name, std::string const& path_list, std::string const& subdir);
            /**
             * Helper function for loadTypekits and loadComponents.
             * @param path_list
             * @param subdir
             * @param kind
             * @return
             */
            void loadComponentsInternal( std::string const& path_list, std::string const& subdir);
        public:
            typedef boost::shared_ptr<ComponentLoader> shared_ptr;
            /**
             * Create the instance of the ComponentLoader. It will keep track
             * of the loaded libraries for this process.
             * @return A singleton.
             */
            static boost::shared_ptr<ComponentLoader> Instance();

            /**
             * Release the ComponentLoader, erasing all knowledge of loaded
             * libraries. No libraries will be unloaded from the process.
             */
            static void Release();

            /**
             * Imports any Component library found in each path in path_list in the current process.
             * This is a best effort function, ie it will silently ignore wrong paths or paths without any
             * Components.
             * @param path_list A colon or semi-colon seperated list of paths
             * to look for Components. May be the empty string.
             */
            void import(std::string const& path_list);

            /**
             * Checks if a given Component type or filename has been imported.
             * This function accepts full filenames ('libthe_Component.so.1.99.0'), short names
             * ('the_Component') or the name provided by the Component Factory ('App::Component').
             * @param type_name name of a file or the Component type.
             * @return true if so.
             */
            bool isImported(std::string type_name);

            /**
             * Imports a Component type found in each path in path_list in the current process.
             * @param name The name of the Component to import, must match the library name (without lib/dll/so pre-/suffixes).
             * @param path_list A colon or semi-colon seperated list of paths
             * to look for Components. May be the empty string.
             */
            bool import(std::string const& name, std::string const& path_list);

            /**
             * Creates a new component an earlier discovered component type.
             * @param name The name of the to be created Component 
             * @param type The type of component to be created.
             * @return null on error or a new component.
             */
            RTT::TaskContext* loadComponent(std::string const& name, std::string const& type);

            /**
             * Destroys an earlier created component.
             * @param tc The TaskContext to be destroyed. tc may no longer
             * be used after this function returns.
             */
            void unloadComponent( RTT::TaskContext* tc );

            /**
             * Lists all Component created by loadComponent().
             * @return A list of Component names
             */
            std::vector<std::string> listComponents() const;

            /**
             * Lists all Component types discovered by the ComponentLoader.
             * @return A list of Component type names
             */
            std::vector<std::string> listComponentTypes() const;

            /**
             * Returns the current Component path list.
             * Defaults to the value of RTT_COMPONENT_PATH, when
             * the RTT was started for the current process.
             * @return A colon separated list of paths or the empty string if not set.
             */
            std::string getComponentPath() const;

            /**
             * Sets the Component path list. This is typically done by RTT
             * startup code with the contents of the RTT_COMPONENT_PATH variable.
             *
             * @param newpath The new paths to look for Components.
             */
            void setComponentPath( std::string const& newpath );
        };
    }
}


#endif /* ORO_COMPONENTLOADER_HPP_ */
