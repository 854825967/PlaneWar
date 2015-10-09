LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../cocos2d/external/sqlite3/include/sqlite3.c \
                   ../../cocos2d/external/sqlite3/include/shell.c \
                   ../../Public/json/json_reader.cpp \
                   ../../Public/json/json_value.cpp \
                   ../../Public/json/json_writer.cpp \
                   ../../Public/tinyxml/tinystr.cpp \
                   ../../Public/tinyxml/tinyxml.cpp \
                   ../../Public/tinyxml/tinyxmlerror.cpp \
                   ../../Public/tinyxml/tinyxmlparser.cpp \
                   ../../Public/LogfileAndroid.cpp \
                   ../../Public/MultiSys.cpp \
                   ../../Public/md5.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/CEventEngine.cpp \
                   ../../Classes/CHttpEngine.cpp \
                   ../../Classes/CTouchEngine.cpp \
                   ../../Classes/Header.cpp \
                   ../../Classes/PublicEvent.cpp \
                   ../../Classes/Accessorie/Bullet.cpp \
                   ../../Classes/Accessorie/Collider.cpp \
                   ../../Classes/Accessorie/Ejector.cpp \
                   ../../Classes/Accessorie/Plane.cpp \
                   ../../Classes/Accessorie/Weapon.cpp \
                   ../../Classes/Accessorie/Wingman.cpp \
                   ../../Classes/Accessorie/Boss.cpp \
                   ../../Classes/Accessorie/Enemy.cpp \
                   ../../Classes/Data/DataManager.cpp \
                   ../../Classes/Data/EquipManager.cpp \
                   ../../Classes/Data/GameConfig.cpp \
                   ../../Classes/Scene/DepotScene_logic.cpp \
                   ../../Classes/Scene/DepotScene.cpp \
                   ../../Classes/Scene/LoginScene.cpp \
                   ../../Classes/Scene/MainMenuScene.cpp \
                   ../../Classes/Scene/MissionScene.cpp \
                   ../../Classes/Scene/BattleScene.cpp \
                   ../../Classes/Scene/LadderScene.cpp \
                   ../../Classes/UI/CBox.cpp \
                   ../../Classes/UI/CButton.cpp \
                   ../../Classes/UI/CEquipView.cpp \
                   ../../Classes/UI/PublicUI.cpp \
                   ../../Classes/UI/ScrollNoticeLayer.cpp \
                   ../../Classes/SDK/SinaWeibo.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Public \
                    $(LOCAL_PATH)/../../Classes/Accessorie \
                    $(LOCAL_PATH)/../../Classes/Data \
                    $(LOCAL_PATH)/../../Classes/Interface \
                    $(LOCAL_PATH)/../../Classes/Scene \
                    $(LOCAL_PATH)/../../Classes/UI

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
$(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
