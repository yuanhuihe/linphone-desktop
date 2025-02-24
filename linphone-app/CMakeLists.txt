################################################################################
#
#  Copyright (c) 2017-2020 Belledonne Communications SARL.
# 
#  This file is part of linphone-desktop
#  (see https://www.linphone.org).
# 
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
# 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>.
#
################################################################################
cmake_minimum_required(VERSION 3.1)

find_package(bctoolbox CONFIG)
set(FULL_VERSION )
bc_compute_full_version(FULL_VERSION)
set(version_major )
set(version_minor )
set(version_patch )
set(identifiers )
set(metadata )
bc_parse_full_version("${FULL_VERSION}" version_major version_minor version_patch identifiers metadata)

project(linphoneqt VERSION "${version_major}.${version_minor}.${version_patch}")


if(ENABLE_BUILD_VERBOSE)
	#message("CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")
	message("Options : ${ENABLE_UPDATE_CHECK}, ${ENABLE_UNIT_TESTS}, ${ENABLE_TESTS}, ${ENABLE_TESTS_COMPONENTS}, ${ENABLE_TOOLS}, ${ENABLE_STRICT}, ${ENABLE_FFMPEG}, ${ENABLE_OPUS}")
endif()
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)


set(TARGET_NAME linphone-qt)
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS true)
set(CMAKE_CXX_STANDARD 11)

if(UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
list(APPEND CMAKE_MODULE_PATH "${LINPHONE_OUTPUT_DIR}/cmake")

set(APP_LIBRARY app-library)
set(APP_PLUGIN app-plugin)
include(application_info.cmake)

if(WIN32)
    set(EXECUTABLE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_DIR} )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_DIR} )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_DIR} )
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )# Apply to all configurations
		string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
		set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_DIR} )
		set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_DIR} )
		set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${EXECUTABLE_OUTPUT_DIR} )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif()

find_package(LinphoneCxx CONFIG)
find_package(Linphone CONFIG)
find_package(belcard CONFIG)
find_package(Mediastreamer2 CONFIG)
find_package(ortp CONFIG)

if(ENABLE_BUILD_VERBOSE)
	message("INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} FRAMEWORK_PATH=${CMAKE_FRAMEWORK_PATH}, PREFIX_PATH=${CMAKE_PREFIX_PATH}")
	message("LINPHONE : ${LINPHONE_INCLUDE_DIRS} => ${LINPHONE_LIBRARIES}")
	message("LINPHONECXX : ${LINPHONECXX_INCLUDE_DIRS} => ${LINPHONECXX_LIBRARIES}")
endif()
# Build configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DQT_NO_DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG" )

if( WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WINSOCKAPI_")#remove error from windows headers order
endif()
set(CMAKE_INCLUDE_CURRENT_DIR ON)#useful for config.h

set(QT5_PACKAGES Core Gui Quick Widgets QuickControls2 Svg LinguistTools Concurrent Network Test)
if (UNIX AND NOT APPLE)
    list(APPEND QT5_PACKAGES DBus)
endif ()
set(QT5_PACKAGES_OPTIONAL TextToSpeech)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS ${QT5_PACKAGES} REQUIRED)
find_package(Qt5 COMPONENTS ${QT5_PACKAGES_OPTIONAL} QUIET)

bc_git_version(${TARGET_NAME} ${PROJECT_VERSION})

#-------------------------------------------------
set(ASSETS_DIR "assets")
set(SOURCES
    src/app/App.cpp
    src/app/AppController.cpp
    src/app/cli/Cli.cpp
    src/app/logger/Logger.cpp
    src/app/paths/Paths.cpp
    src/app/providers/AvatarProvider.cpp
    src/app/providers/ImageProvider.cpp
    src/app/providers/ExternalImageProvider.cpp
    src/app/providers/ThumbnailProvider.cpp
    src/app/translator/DefaultTranslator.cpp
    src/components/assistant/AssistantModel.cpp
    src/components/authentication/AuthenticationNotifier.cpp
    src/components/call/CallModel.cpp
    src/components/calls/CallsListModel.cpp
    src/components/calls/CallsListProxyModel.cpp
    src/components/camera/Camera.cpp
    src/components/camera/CameraPreview.cpp
    src/components/chat/ChatModel.cpp
    src/components/chat/ChatProxyModel.cpp
    src/components/codecs/AbstractCodecsModel.cpp
    src/components/codecs/AudioCodecsModel.cpp
    src/components/codecs/VideoCodecsModel.cpp
    src/components/conference/ConferenceAddModel.cpp
    src/components/conference/ConferenceHelperModel.cpp
    src/components/conference/ConferenceModel.cpp
    src/components/contact/ContactModel.cpp
    src/components/contact/VcardModel.cpp
    src/components/contacts/ContactsImporterModel.cpp
    src/components/contacts/ContactsImporterPluginsManager.cpp
    src/components/contacts/ContactsImporterListModel.cpp
    src/components/contacts/ContactsImporterListProxyModel.cpp
    src/components/contacts/ContactsListModel.cpp
    src/components/contacts/ContactsListProxyModel.cpp
    src/components/core/CoreHandlers.cpp
    src/components/core/CoreManager.cpp
    src/components/core/event-count-notifier/AbstractEventCountNotifier.cpp
    src/components/file/FileDownloader.cpp
    src/components/file/FileExtractor.cpp
    src/components/history/HistoryModel.cpp
    src/components/history/HistoryProxyModel.cpp
    src/components/ldap/LdapModel.cpp
    src/components/ldap/LdapListModel.cpp
    src/components/ldap/LdapProxyModel.cpp
    src/components/notifier/Notifier.cpp
    src/components/other/clipboard/Clipboard.cpp
    src/components/other/colors/Colors.cpp
    src/components/other/text-to-speech/TextToSpeech.cpp
    src/components/other/units/Units.cpp
    src/components/presence/OwnPresenceModel.cpp
    src/components/presence/Presence.cpp
    src/components/search/SearchHandler.cpp
    src/components/settings/AccountSettingsModel.cpp
    src/components/settings/SettingsModel.cpp
    src/components/sip-addresses/SipAddressesModel.cpp
    src/components/sip-addresses/SipAddressesProxyModel.cpp
    src/components/sip-addresses/SipAddressObserver.cpp
    src/components/sip-addresses/SearchSipAddressesModel.cpp
    src/components/sound-player/SoundPlayer.cpp
    src/components/telephone-numbers/TelephoneNumbersModel.cpp
    src/components/timeline/TimelineModel.cpp
    src/components/url-handlers/UrlHandlers.cpp
    src/utils/LinphoneUtils.cpp
    src/utils/MediastreamerUtils.cpp
    src/utils/QExifImageHeader.cpp
    src/utils/Utils.cpp
	src/utils/plugins/PluginsManager.cpp
    )
set(PLUGIN_SOURCES src/utils/plugins/PluginDataAPI.cpp
	src/utils/plugins/PluginNetworkHelper.cpp
    src/utils/plugins/LinphonePlugin.cpp
	)

set(HEADERS
    src/app/App.hpp
    src/app/AppController.hpp
    src/app/cli/Cli.hpp
    src/app/logger/Logger.hpp
    src/app/paths/Paths.hpp
    src/app/providers/AvatarProvider.hpp
    src/app/providers/ImageProvider.hpp
    src/app/providers/ExternalImageProvider.hpp
    src/app/providers/ThumbnailProvider.hpp
    src/app/single-application/SingleApplication.hpp
    src/app/translator/DefaultTranslator.hpp
    src/components/assistant/AssistantModel.hpp
    src/components/authentication/AuthenticationNotifier.hpp
    src/components/call/CallModel.hpp
    src/components/calls/CallsListModel.hpp
    src/components/calls/CallsListProxyModel.hpp
    src/components/camera/Camera.hpp
    src/components/camera/CameraPreview.hpp
    src/components/chat/ChatModel.hpp
    src/components/chat/ChatProxyModel.hpp
    src/components/codecs/AbstractCodecsModel.hpp
    src/components/codecs/AudioCodecsModel.hpp
    src/components/codecs/VideoCodecsModel.hpp
    src/components/Components.hpp
    src/components/conference/ConferenceAddModel.hpp
    src/components/conference/ConferenceHelperModel.hpp
    src/components/conference/ConferenceModel.hpp
    src/components/contact/ContactModel.hpp
    src/components/contact/VcardModel.hpp
    src/components/contacts/ContactsImporterModel.hpp
    src/components/contacts/ContactsImporterPluginsManager.hpp
    src/components/contacts/ContactsImporterListModel.hpp
    src/components/contacts/ContactsImporterListProxyModel.hpp
    src/components/contacts/ContactsListModel.hpp
    src/components/contacts/ContactsListProxyModel.hpp
    src/components/core/CoreHandlers.hpp
    src/components/core/CoreManager.hpp
    src/components/core/event-count-notifier/AbstractEventCountNotifier.hpp
    src/components/file/FileDownloader.hpp
    src/components/file/FileExtractor.hpp
    src/components/history/HistoryModel.hpp
    src/components/history/HistoryProxyModel.hpp
    src/components/ldap/LdapModel.hpp
    src/components/ldap/LdapListModel.hpp
    src/components/ldap/LdapProxyModel.hpp
    src/components/notifier/Notifier.hpp
    src/components/other/clipboard/Clipboard.hpp
    src/components/other/colors/Colors.hpp
    src/components/other/desktop-tools/DesktopTools.hpp
    src/components/other/text-to-speech/TextToSpeech.hpp
    src/components/other/units/Units.hpp
    src/components/presence/OwnPresenceModel.hpp
    src/components/presence/Presence.hpp
    src/components/search/SearchHandler.hpp
    src/components/settings/AccountSettingsModel.hpp
    src/components/settings/SettingsModel.hpp
    src/components/sip-addresses/SipAddressesModel.hpp
    src/components/sip-addresses/SipAddressesProxyModel.hpp
    src/components/sip-addresses/SipAddressObserver.hpp
    src/components/sip-addresses/SearchSipAddressesModel.hpp
    src/components/sound-player/SoundPlayer.hpp
    src/components/telephone-numbers/TelephoneNumbersModel.hpp
    src/components/timeline/TimelineModel.hpp
    src/components/url-handlers/UrlHandlers.hpp
    src/utils/LinphoneUtils.hpp
    src/utils/MediastreamerUtils.hpp
    src/utils/QExifImageHeader.hpp
    src/utils/Utils.hpp
    src/utils/plugins/PluginsManager.hpp
    )
set(PLUGIN_HEADERS 
	include/LinphoneApp/PluginDataAPI.hpp
    include/LinphoneApp/PluginNetworkHelper.hpp
	include/LinphoneApp/LinphonePlugin.hpp)
list(APPEND SOURCES include/LinphoneApp/PluginExample.json)
set(MAIN_FILE src/app/main.cpp)

if (APPLE)
    list(APPEND SOURCES
    src/app/single-application/SingleApplication.cpp
    src/components/core/event-count-notifier/EventCountNotifierMacOs.m
    src/components/other/desktop-tools/DesktopToolsMacOs.cpp
    src/components/other/desktop-tools/DesktopToolsMacOs.mm
    src/components/other/desktop-tools/screen-saver/ScreenSaverMacOs.m
    src/components/other/desktop-tools/state-process/StateProcessMacOs.mm
    )
    list(APPEND HEADERS
    src/app/single-application/SingleApplicationPrivate.hpp
    src/components/core/event-count-notifier/EventCountNotifierMacOs.hpp
    src/components/other/desktop-tools/DesktopToolsMacOs.hpp
    )
elseif (WIN32)
    list(APPEND SOURCES
    src/app/single-application/SingleApplication.cpp
    src/components/core/event-count-notifier/EventCountNotifierSystemTrayIcon.cpp
    src/components/other/desktop-tools/DesktopToolsWindows.cpp
    )
    list(APPEND HEADERS
    src/app/single-application/SingleApplicationPrivate.hpp
    src/components/core/event-count-notifier/EventCountNotifierSystemTrayIcon.hpp
    src/components/other/desktop-tools/DesktopToolsWindows.hpp
    )
else ()
    list(APPEND SOURCES
    src/app/single-application/SingleApplicationDBus.cpp
    src/components/core/event-count-notifier/EventCountNotifierSystemTrayIcon.cpp
    src/components/other/desktop-tools/DesktopToolsLinux.cpp
    src/components/other/desktop-tools/screen-saver/ScreenSaverDBus.cpp
    src/components/other/desktop-tools/screen-saver/ScreenSaverXdg.cpp
    )
    list(APPEND HEADERS
    src/app/single-application/SingleApplicationDBusPrivate.hpp
    src/components/core/event-count-notifier/EventCountNotifierSystemTrayIcon.hpp
    src/components/other/desktop-tools/DesktopToolsLinux.hpp
    src/components/other/desktop-tools/screen-saver/ScreenSaverDBus.hpp
    src/components/other/desktop-tools/screen-saver/ScreenSaverXdg.hpp
    )
endif ()

set(QRC_RESOURCES resources.qrc)

#qt5_add_big_resources(QRC_BIG_RESOURCES resources.qrc)	#Use this macro if resources size is too high

set(LANGUAGES_DIRECTORY "${ASSETS_DIR}/languages")
set(I18N_FILENAME i18n.qrc)
set(LANGUAGES da de en es fr_FR hu it ja lt pt_BR ru sv tr uk zh_CN)

# ------------------------------------------------------------------------------

function (PREPEND list prefix)
    set(new_list "")

    foreach (elem ${${list}})
    list(APPEND new_list "${prefix}${elem}")
    endforeach ()

    set(${list} ${new_list} PARENT_SCOPE)
endfunction ()

# Force absolute paths.
PREPEND(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/")
PREPEND(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/")
PREPEND(PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/")
PREPEND(PLUGIN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/")

# ------------------------------------------------------------------------------
# Compute QML files list.
# ------------------------------------------------------------------------------

set(QML_SOURCES)
file(STRINGS ${QRC_RESOURCES} QRC_RESOURCES_CONTENT)
foreach (line ${QRC_RESOURCES_CONTENT})
    set(result)
    string(REGEX REPLACE
    "^[ \t]*<[ \t]*file[ \t]*>[ \t]*(.+\\.[a-z]+)[ \t]*<[ \t]*/[ \t]*file[ \t]*>[ \t]*$"
    "\\1"
    result
    "${line}"
    )
    string(REGEX MATCH "\\.[a-z]+$" is_ui ${result})
    if (NOT ${is_ui} STREQUAL "")
    list(APPEND QML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${result}")
    endif ()
endforeach ()

# ------------------------------------------------------------------------------
# Init git hooks.
# ------------------------------------------------------------------------------

if (NOT WIN32)
    add_custom_target(
    check_qml DEPENDS ${QML_SOURCES}
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../tools/check_qml_syntax"
    )
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/../tools/private/pre-commit"
    "${CMAKE_CURRENT_SOURCE_DIR}/../.git/hooks/pre-commit"
    )
set(_QML_IMPORT_PATHS "")
list(APPEND _QML_IMPORT_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/modules")
list(APPEND _QML_IMPORT_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/dev-modules")
list(APPEND _QML_IMPORT_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/scripts")
list(APPEND _QML_IMPORT_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/views")


set(QML_IMPORT_PATH "${_QML_IMPORT_PATHS}" CACHE STRING "Path used to locate CMake modules by Qt Creator" FORCE)
set(QML2_IMPORT_PATH "${_QML_IMPORT_PATHS}" CACHE STRING "Path used to locate CMake modules by Qt Creator" FORCE)
set(QML_SOURCES_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/")
set(QML_MODULES_PATHS ${QML_SOURCES_PATHS})
if(ENABLE_BUILD_VERBOSE)#useful to copy these Paths to QML previewers
    message("QML_IMPORT_PATH=${QML_IMPORT_PATH}" )
    message("Qt5 Paths : Qt5_DIR=$ENV{Qt5_DIR}, PATH=$ENV{PATH}")
endif()
if(APPLE)
    if(MS2_PLUGINS_LOCATION)
        set(MSPLUGINS_DIR ${MS2_PLUGINS_LOCATION})
    else()
        set(MSPLUGINS_DIR "Frameworks/mediastreamer2.framework/Versions/A/Libraries")
    endif()
else()
    set(MSPLUGINS_DIR "plugins/mediastreamer")
endif()
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")
# ------------------------------------------------------------------------------
# Build.
# ------------------------------------------------------------------------------

include_directories(src/)
include_directories("${LINPHONE_OUTPUT_DIR}/include/OpenGL")

if (CMAKE_INSTALL_RPATH)
    #Retrieve lib path from a know QT executable
    get_target_property(LUPDATE_PATH Qt5::lupdate LOCATION)
    get_filename_component(LUPDATE_PATH "${LUPDATE_PATH}" DIRECTORY)
    get_filename_component(QT_PATH "${LUPDATE_PATH}/lib" ABSOLUTE)
    list(APPEND CMAKE_INSTALL_RPATH "${QT_PATH}")
endif ()
# Add languages support.
add_subdirectory("${LANGUAGES_DIRECTORY}" "assets/languages")
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${LANGUAGES_DIRECTORY}/${I18N_FILENAME}")
get_directory_property(TS_FILES DIRECTORY "${LANGUAGES_DIRECTORY}"  DEFINITION TS_FILES)
list(APPEND SOURCES ${TS_FILES})

# set application details
if(WIN32)
	string(TIMESTAMP CURRENT_YEAR "%Y")
	set(COPYRIGTH_RANGE_DATE "2010-${CURRENT_YEAR}")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_builder/linphone_package/windows/appDetailsWindows.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/appDetailsWindows.rc")
	set(RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/appDetailsWindows.rc)
endif()

add_library(${APP_PLUGIN} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})
add_library(${APP_LIBRARY} OBJECT ${SOURCES} ${HEADERS} ${QML_SOURCES} ${QRC_RESOURCES} ${PLUGIN_HEADERS})#Need to add Headers to resolve moc Qt symbols

if (WIN32)
    add_executable(${TARGET_NAME} WIN32 $<TARGET_OBJECTS:${APP_LIBRARY}> ${MAIN_FILE} ${QRC_BIG_RESOURCES} ${RC_FILE})
    install(TARGETS linphone-qt RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    if(MSVC AND (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
        install(FILES "$<TARGET_PDB_FILE:${TARGET_NAME}>" DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
elseif (APPLE)
    add_executable(${TARGET_NAME} $<TARGET_OBJECTS:${APP_LIBRARY}> ${MAIN_FILE} ${QRC_BIG_RESOURCES})
    install(TARGETS linphone-qt RUNTIME DESTINATION "${APPLICATION_NAME}.app/Contents/MacOS") # using ${TARGET_NAME} lead to cmake error on target name. Use directly linphone-qt.
else ()
    add_executable(${TARGET_NAME} $<TARGET_OBJECTS:${APP_LIBRARY}> ${MAIN_FILE} ${QRC_BIG_RESOURCES})
    install(TARGETS linphone-qt RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif ()


set_property(TARGET ${APP_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE ON)   #Need by Qt
set_property(TARGET ${APP_PLUGIN} PROPERTY POSITION_INDEPENDENT_CODE ON)   #Need by Qt


#Turn on automatic resources compilation by cmake
#Instead of excplicitely calling qt5_add_resources
set_property(TARGET ${APP_LIBRARY} PROPERTY AUTORCC ON)
set_property(TARGET ${APP_PLUGIN} PROPERTY AUTORCC ON)



set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${EXECUTABLE_NAME}")

if(MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES PDB_NAME "${EXECUTABLE_NAME}_app")
endif()


target_compile_definitions(${APP_PLUGIN} PUBLIC "-DENABLE_APP_EXPORT_PLUGIN")
set_target_properties(${APP_PLUGIN} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(INCLUDED_DIRECTORIES "${LINPHONECXX_INCLUDE_DIRS}" "${MEDIASTREAMER2_INCLUDE_DIRS}")
list(APPEND INCLUDED_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/include")

set(LIBRARIES_LIST ${BCTOOLBOX_CORE_LIBRARIES} ${BELCARD_LIBRARIES}  ${LINPHONE_LIBRARIES} ${LINPHONECXX_LIBRARIES} ${MEDIASTREAMER2_LIBRARIES} ${ORTP_LIBRARIES} ${OPUS_LIBRARIES})
if(WIN32)
	set(LIBRARIES)
	foreach(LIBRARY ${LIBRARIES_LIST})# Search for lib full path
		find_library(FIND_LIBRARY_ITEM_${LIBRARY} NAMES ${LIBRARY} lib${LIBRARY} REQUIRED)#find_library need a specific variable name each time
		if(FIND_LIBRARY_ITEM_${LIBRARY})
			list(APPEND LIBRARIES ${FIND_LIBRARY_ITEM_${LIBRARY}})
		else()
			message(SEND_ERROR "${LIBRARY} not found!")
		endif()
	endforeach()
else()
	set(LIBRARIES ${LIBRARIES_LIST})
endif()

if(ENABLE_BUILD_VERBOSE)
	message("LIBRARIES : ${LIBRARIES}")
endif()
foreach (package ${QT5_PACKAGES})
    list(APPEND INCLUDED_DIRECTORIES "${Qt5${package}_INCLUDE_DIRS}")
    # `qt5_create_translation` is provided from `LinguistTools` package.
    # But the `Qt5::LinguistTools` lib does not exist. Remove it.
    if (NOT (${package} STREQUAL LinguistTools))
    list(APPEND LIBRARIES ${Qt5${package}_LIBRARIES})
    endif ()
endforeach ()

foreach (package ${QT5_PACKAGES_OPTIONAL})
    if ("${Qt5${package}_FOUND}")
    message("Optional package ${package} found.")
    list(APPEND INCLUDED_DIRECTORIES "${Qt5${package}_INCLUDE_DIRS}")
    list(APPEND LIBRARIES ${Qt5${package}_LIBRARIES})

    string(TOUPPER "${package}" INCLUDE_NAME)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${INCLUDE_NAME}_ENABLED")
    else ()
    message("Optional package ${package} not found.")
    endif ()
endforeach ()


if (APPLE)
    list(APPEND LIBRARIES "-framework Cocoa -framework IOKit -framework AVFoundation")
    # -framework linphone") #This doesn't work yet
endif ()

target_include_directories(${APP_LIBRARY} SYSTEM PUBLIC ${INCLUDED_DIRECTORIES})
target_include_directories(${APP_PLUGIN} SYSTEM PUBLIC ${INCLUDED_DIRECTORIES})
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC ${INCLUDED_DIRECTORIES})

target_link_libraries(${APP_LIBRARY} ${LIBRARIES})
target_link_libraries(${APP_PLUGIN} ${LIBRARIES})
target_link_libraries(${APP_LIBRARY} ${APP_PLUGIN})
target_link_libraries(${TARGET_NAME} ${LIBRARIES})
target_link_libraries(${TARGET_NAME} ${APP_PLUGIN})

if(WIN32)
	find_library(LDAP_LIBRARIES NAMES ldap libldap HINTS "${LINPHONE_OUTPUT_DIR}/lib")
	find_library(LBER_LIBRARIES NAMES lber liblber HINTS "${LINPHONE_OUTPUT_DIR}/lib")
	target_link_libraries(${TARGET_NAME} wsock32 ws2_32 ${LDAP_LIBRARIES} ${LBER_LIBRARIES})
endif()

add_dependencies(${APP_LIBRARY}  update_translations ${TARGET_NAME}-git-version ${APP_PLUGIN})
add_dependencies(${TARGET_NAME} ${APP_LIBRARY} ${APP_PLUGIN})


# ------------------------------------------------------------------------------
# CPack settings & RPM.
# ------------------------------------------------------------------------------
set(TOOLS_DIR "${CMAKE_BINARY_DIR}/programs")
set(LINPHONE_BUILDER_SIGNING_IDENTITY ${LINPHONE_BUILDER_SIGNING_IDENTITY})

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/include/" "${CMAKE_INSTALL_PREFIX}/include/")
add_custom_command(TARGET ${APP_PLUGIN} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_PREFIX}/lib/")
add_custom_command(TARGET ${APP_PLUGIN} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${APP_PLUGIN}> "${CMAKE_INSTALL_PREFIX}/lib/")
add_custom_command(TARGET ${APP_PLUGIN} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:${APP_PLUGIN}> "${CMAKE_INSTALL_PREFIX}/lib/")
#add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/include/LinphoneApp/*" "${CMAKE_INSTALL_PREFIX}/include/LinphoneApp/")
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/*" "${CMAKE_INSTALL_PREFIX}/include/LinphoneApp/" COPYONLY)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION ".")


add_subdirectory(build)
add_subdirectory(cmake_builder/linphone_package)


# ------------------------------------------------------------------------------
# To start better integration into IDE.
# ------------------------------------------------------------------------------
source_group(
    "Json"  REGULAR_EXPRESSION ".+\.json$"
)
source_group(
    "Qml" REGULAR_EXPRESSION ".+\.qml$"
    )
source_group(
    "Js" REGULAR_EXPRESSION ".+\.js$"
    )
source_group(
    "Svg" REGULAR_EXPRESSION ".+\.svg$"
    )
source_group(
    "Lang" REGULAR_EXPRESSION ".+\.ts$"
    )
source_group(
    "Rc" REGULAR_EXPRESSION ".+\.rc$"
    )
