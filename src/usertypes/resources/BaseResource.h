/// @author James Holtom

#ifndef BASE_RESOURCE_H
#define BASE_RESOURCE_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include "../Flaggable.h"

namespace term_engine::usertypes {
  class BaseResource;

  /// @brief Shared pointer to an resource.
  typedef std::unique_ptr<BaseResource> ResourcePtr;
  /// @brief Used to store a list of resources.
  typedef std::unordered_map<std::string, ResourcePtr> ResourceList;
  /// @brief Used to pass either a BaseResource object or it's string index to functions.
  typedef std::variant<BaseResource*, std::string> BaseResourceVariant;

  /// @brief The list of resources.
  inline ResourceList resource_list;

  /// @brief The base resource, on which other game resources are derived from.
  class BaseResource : public Flaggable {
  public:
    /**
     * @brief Constructs the base resource with the given name/filepath.
     * 
     * @param[in] name The name/filepath to the resource.
     */
    BaseResource(const std::string& name);

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    virtual std::string GetResourceType() const = 0;

    /**
     * @brief Returns the name of the resource.
     * 
     * @returns The resource name.
     */
    std::string GetName() const;

    /// @brief Updates the debugging information for this resource.
    virtual void UpdateDebugInfo() const = 0;

  protected:
    /// @brief The name/filepath of the resource.
    std::string name_;
  };

  /**
   * @brief Removes the given resource from the list.
   * 
   * @param[in] resource The resource, or it's string index.
   */
  void RemoveResource(BaseResourceVariant resource);

  /// @brief Removes all resources in the list.
  void CleanUpResources();

  /// @brief Clears all resources that are flagged for removal from the list.
  void ClearFlaggedResources();
}

#endif // ! BASE_RESOURCE_H