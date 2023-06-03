/// @author James Holtom

#ifndef BASE_RESOURCE_H
#define BASE_RESOURCE_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace term_engine::usertypes {
  class BaseResource;

  /// @brief Shared pointer to an resource.
  typedef std::unique_ptr<BaseResource> ResourcePtr;
  /// @brief Used to store a list of resources.
  typedef std::unordered_map<std::string, ResourcePtr> ResourceList;

  /// @brief The list of resources.
  inline ResourceList resource_list;

  /// @brief The base resource, on which other game resources are derived from.
  class BaseResource {
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

    /**
     * @brief Returns if the resource is flagged to be removed.
     * 
     * @returns If the resource is flagged to be removed.
     */
    bool FlaggedForRemoval() const;

    /// @brief Flags the resource to be removed.
    void FlagForRemoval();

    /// @brief Unsets the removal flag from the resource.
    void UnflagForRemoval();

    /// @brief Updates the debugging information for this resource.
    virtual void UpdateDebugInfo() const = 0;

  protected:
    /// @brief The name/filepath of the resource.
    std::string name_;
    /// @brief A flag to mark this resource to be removed.
    bool marked_for_removal_;
  };

  void UpdateResources();

  /// @brief Removes all resources in the list.
  void CleanUpResources();

  /// @brief Clears all resources that are flagged for removal from the resource list.
  void ClearFlaggedResources();
}

#endif // ! BASE_RESOURCE_H