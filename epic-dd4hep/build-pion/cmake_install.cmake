# Install script for directory: /home/devesh/eic/epic-dd4hep

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/devesh/eic/epic-dd4hep/install-pion")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/lib/libepic.components")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so"
         RPATH "/home/devesh/eic/epic-dd4hep/install-pion/lib:/opt/software/linux-x86_64_v2/root-6.40.04-xv2nd6wrx33iv3yu4eoyauy4smm2pa2n/lib/root:/opt/software/linux-x86_64_v2/dd4hep-1.38-lxlhfjfu2zhsbd2h6hfkzssmjzb3ltsb/lib:/opt/software/linux-x86_64_v2/fmt-11.2.0-b3slvntzago7c3hfa7lt7zizycojn7lt/lib:/opt/software/linux-x86_64_v2/irt2-2.2.3-tvtayzi3u2qbzhlcemclhlffd7bcdumh/lib:/opt/software/linux-x86_64_v2/xerces-c-3.3.0-fcfj4uvcqpmvxoesbskrhlkch4bjwv3c/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/devesh/eic/epic-dd4hep/build-pion/lib/libepic.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so"
         OLD_RPATH "/opt/software/linux-x86_64_v2/root-6.40.04-xv2nd6wrx33iv3yu4eoyauy4smm2pa2n/lib/root:/opt/software/linux-x86_64_v2/dd4hep-1.38-lxlhfjfu2zhsbd2h6hfkzssmjzb3ltsb/lib:/opt/software/linux-x86_64_v2/fmt-11.2.0-b3slvntzago7c3hfa7lt7zizycojn7lt/lib:/opt/software/linux-x86_64_v2/irt2-2.2.3-tvtayzi3u2qbzhlcemclhlffd7bcdumh/lib:/opt/software/linux-x86_64_v2/xerces-c-3.3.0-fcfj4uvcqpmvxoesbskrhlkch4bjwv3c/lib::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/home/devesh/eic/epic-dd4hep/install-pion/lib:/opt/software/linux-x86_64_v2/root-6.40.04-xv2nd6wrx33iv3yu4eoyauy4smm2pa2n/lib/root:/opt/software/linux-x86_64_v2/dd4hep-1.38-lxlhfjfu2zhsbd2h6hfkzssmjzb3ltsb/lib:/opt/software/linux-x86_64_v2/fmt-11.2.0-b3slvntzago7c3hfa7lt7zizycojn7lt/lib:/opt/software/linux-x86_64_v2/irt2-2.2.3-tvtayzi3u2qbzhlcemclhlffd7bcdumh/lib:/opt/software/linux-x86_64_v2/xerces-c-3.3.0-fcfj4uvcqpmvxoesbskrhlkch4bjwv3c/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libepic.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_backward_hcal_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_backward_hcal_only_sampF.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_bhcal.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_calorimeters.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x100.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x100_Au197.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x110_He3.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x115_Cu63.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x115_Ru96.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x130.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x130_H2.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x166_He3.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x250.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_10x275.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_18x110_Au.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_18x110_He3.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_18x275.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_5x100.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_5x41.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_5x41_He3.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_bic_6layers.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_bic_layer1_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_material_map.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_no_bhcal.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_no_zdc_lyso.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_tracking_2DStrip.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_tracking_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_without_zdc_10x100_Au197.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_craterlake_without_zdc_5x41_Au197.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_dirc_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_drich_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_eeemcal_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_femcal_averaged_homogeneous.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_femcal_scfi.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_fhcal.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_forward_calorimeters.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_forward_detectors.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_forward_detectors_with_inserts.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_full.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_imaging_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_inner_detector.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_ip6.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_ip6_extended.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_lfhcal_2024_TB.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_lfhcal_2025_TB.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_lfhcal_2026_TB_opt1.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_lfhcal_2026_TB_opt2.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_lfhcal_2028_TB_opt2.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_lfhcal_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_pfrich_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_pid_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_tof_endcap_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_tof_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_tracking_and_drich.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_tracking_and_pfrich.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_vertex_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_zdc_lyso_sipm.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_zdc_sipm_on_tile_only.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic" TYPE FILE RENAME "epic.xml" FILES "/home/devesh/eic/epic-dd4hep/build-pion/epic_full.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic/compact" TYPE DIRECTORY FILES "/home/devesh/eic/epic-dd4hep/compact/" FILES_MATCHING REGEX "/[^/]*\\.xml$" REGEX "/[^/]*\\.gdml$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic/calibrations" TYPE DIRECTORY FILES "/home/devesh/eic/epic-dd4hep/calibrations/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/epic/compact" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/version.xml")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "/home/devesh/eic/epic-dd4hep/build-pion/thisepic.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/devesh/eic/epic-dd4hep/install-pion/setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/devesh/eic/epic-dd4hep/install-pion" TYPE FILE RENAME "setup.sh" FILES "/home/devesh/eic/epic-dd4hep/build-pion/thisepic.sh")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "/home/devesh/eic/epic-dd4hep/bin/g4MaterialScan_to_csv")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/devesh/eic/epic-dd4hep/build-pion/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/devesh/eic/epic-dd4hep/build-pion/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
