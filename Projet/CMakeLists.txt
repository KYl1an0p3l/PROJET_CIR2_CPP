# CMakeList.txt : projet CMake pour Projet, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Projet "Projet.cpp" "Projet.h" "map.cpp" "map.h")

#cibles
target_link_libraries(Projet sfml-system sfml-window sfml-graphics sfml-audio)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Projet PROPERTY CXX_STANDARD 20)
endif()