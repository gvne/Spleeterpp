#ifndef SPEETER_SPLEETER_H_
#define SPEETER_SPLEETER_H_

#include <string>
#include <memory>
#include <unordered_set>
#include <Eigen/Core>

#include "spleeter/type.h"
#include "spleeter/filter.h"

namespace spleeter {

/// Initialize the spleeter tools by loading models at the specified path
/// \param path_to_models the path to spleeter exported models.
/// \param separation_types the types of separation required
/// \param err an error code that gets set if something goes wrong
void Initialize(const std::string &path_to_models,
                const std::unordered_set<SeparationType> &separation_types,
                std::error_code &err);

} // namespace spleeter

#endif // SPEETER_SPLEETER_H_
