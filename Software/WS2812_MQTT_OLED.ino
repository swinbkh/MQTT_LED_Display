#define will_topic "stat/LED_Display_1/Status" //Where the board will post Online/Offline. Useful as a trigger to re-send data after disconnected

const char* WIFI_SSID = "PUT_YOUR_SSID_HERE";
const char* WIFI_PWD = "AND_YOUR_PASSWORD_HERE";
#define mqtt_server     "192.168.1.2" //CHANGE TO YOUR MQTT IP

#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>
#include <Wire.h>
#include "icons.h"

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 5
#define DATA_PIN 14 //D5
CRGB leds[NUM_LEDS];


WiFiClient espClient;
PubSubClient client(espClient);

#define mqtt_user       "MQTT-login"
#define mqtt_password   "MQTT-password"
#define will_message    "Offline"
#define lwt_msg         "Online"
#define will_QoS        0
#define will_retain     true

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

int flash_on = LOW;             // Is the flash state currently on?
unsigned long previousMillis = 0;        // will store last time LED was updated
const long flash_interval = 1000;           // interval at which to blink (milliseconds)

float LED1_H = 0, LED2_H = 00, LED3_H = 60, LED4_H = 96, LED5_H = 96;
float LED1_S = 255, LED2_S = 255, LED3_S = 255, LED4_S = 255, LED5_S = 255;
float LED1_V = 50, LED2_V = 50, LED3_V = 50, LED4_V = 50, LED5_V = 50;
//bool LED1_FLASH = false, LED2_FLASH = false, LED3_FLASH = false, LED4_FLASH = false, LED5_FLASH = false;
bool FlashArray[5] = {false, false, false, false, false};

SSD1306Wire  display(0x3c, 4, 5);
OLEDDisplayUi ui     ( &display );

String Page1Title = "Page1";
String Page1Subtext = "Page1Subtext";
String Page1Icon;

String Page2Title = "Page2";
String Page2Subtext = "Page2Subtext";
String Page2Icon;

String Page3Title = "Page3";
String Page3Subtext = "Page3Subtext";
String Page3Icon;

String Page4Title = "Page4";
String Page4Subtext = "Page4Subtext";
String Page4Icon;

String Page5Title = "Page5";
String Page5Subtext = "Page5Subtext";
String Page5Icon;

String IconArray[] = {
  "IMAGE_NOT_FOUND",
  "NONE",
  "3D_ROTATION",
  "ACCESSIBILITY",
  "ACCESSIBLE",
  "ACCOUNT_BALANCE",
  "ACCOUNT_BALANCE_WALLET",
  "ACCOUNT_BOX",
  "ACCOUNT_CIRCLE",
  "AC_UNIT",
  "ADB",
  "ADD_ALERT",
  "ADD_A_PHOTO",
  "ADD",
  "ADD_BOX",
  "ADD_CIRCLE",
  "ADD_CIRCLE_OUTLINE",
  "ADD_LOCATION",
  "ADD_SHOPPING_CART",
  "ADD_TO_PHOTOS",
  "ADD_TO_QUEUE",
  "ADJUST",
  "AIRLINE_SEAT_FLAT_ANGLED",
  "AIRLINE_SEAT_FLAT",
  "AIRLINE_SEAT_INDIVIDUAL_SUITE",
  "AIRLINE_SEAT_LEGROOM_EXTRA",
  "AIRLINE_SEAT_LEGROOM_NORMAL",
  "AIRLINE_SEAT_LEGROOM_REDUCED",
  "AIRLINE_SEAT_RECLINE_EXTRA",
  "AIRLINE_SEAT_RECLINE_NORMAL",
  "AIRPLAY",
  "AIRPORT_SHUTTLE",
  "ALARM_ADD",
  "ALARM",
  "ALARM_OFF",
  "ALARM_ON",
  "ALBUM",
  "ALL_INCLUSIVE",
  "ALL_OUT",
  "ANDROID",
  "ANNOUNCEMENT",
  "APPS",
  "ARCHIVE",
  "ARROW_BACK",
  "ARROW_DOWNWARD",
  "ARROW_DROP_DOWN",
  "ARROW_DROP_DOWN_CIRCLE",
  "ARROW_DROP_UP",
  "ARROW_FORWARD",
  "ARROW_UPWARD",
  "ART_TRACK",
  "ASPECT_RATIO",
  "ASSESSMENT",
  "ASSIGNMENT",
  "ASSIGNMENT_IND",
  "ASSIGNMENT_LATE",
  "ASSIGNMENT_RETURNED",
  "ASSIGNMENT_RETURN",
  "ASSIGNMENT_TURNED_IN",
  "ASSISTANT",
  "ASSISTANT_PHOTO",
  "ATTACHMENT",
  "ATTACH_FILE",
  "ATTACH_MONEY",
  "AUDIOTRACK",
  "AUTORENEW",
  "AV_TIMER",
  "BACKSPACE",
  "BACKUP",
  "BEACH_ACCESS",
  "BEENHERE",
  "BLOCK",
  "BLUETOOTH_AUDIO",
  "BLUR_CIRCULAR",
  "BLUR_LINEAR",
  "BLUR_OFF",
  "BLUR_ON",
  "BOOKMARK",
  "BOOKMARK_BORDER",
  "BOOK",
  "BORDER_ALL",
  "BORDER_BOTTOM",
  "BORDER_CLEAR",
  "BORDER_COLOR",
  "BORDER_HORIZONTAL",
  "BORDER_INNER",
  "BORDER_LEFT",
  "BORDER_OUTER",
  "BORDER_RIGHT",
  "BORDER_STYLE",
  "BORDER_TOP",
  "BORDER_VERTICAL",
  "BRANDING_WATERMARK",
  "BRIGHTNESS_1",
  "BRIGHTNESS_2",
  "BRIGHTNESS_3",
  "BRIGHTNESS_4",
  "BRIGHTNESS_5",
  "BRIGHTNESS_6",
  "BRIGHTNESS_7",
  "BROKEN_IMAGE",
  "BRUSH",
  "BUBBLE_CHART",
  "BUG_REPORT",
  "BUILD",
  "BURST_MODE",
  "BUSINESS",
  "BUSINESS_CENTER",
  "CACHED",
  "CAKE",
  "CALL",
  "CALL_END",
  "CALL_MADE",
  "CALL_MERGE",
  "CALL_MISSED",
  "CALL_MISSED_OUTGOING",
  "CALL_RECEIVED",
  "CALL_SPLIT",
  "CALL_TO_ACTION",
  "CAMERA_ALT",
  "CAMERA",
  "CAMERA_ENHANCE",
  "CAMERA_FRONT",
  "CAMERA_REAR",
  "CAMERA_ROLL",
  "CANCEL",
  "CARD_GIFTCARD",
  "CARD_MEMBERSHIP",
  "CARD_TRAVEL",
  "CASINO",
  "CAST",
  "CAST_CONNECTED",
  "CENTER_FOCUS_STRONG",
  "CENTER_FOCUS_WEAK",
  "CHANGE_HISTORY",
  "CHAT",
  "CHAT_BUBBLE",
  "CHAT_BUBBLE_OUTLINE",
  "CHECK",
  "CHECK_BOX",
  "CHECK_BOX_OUTLINE_BLANK",
  "CHECK_CIRCLE",
  "CHEVRON_LEFT",
  "CHEVRON_RIGHT",
  "CHILD_CARE",
  "CHILD_FRIENDLY",
  "CHROME_READER_MODE",
  "CLASS",
  "CLEAR_ALL",
  "CLEAR",
  "CLOSED_CAPTION",
  "CLOSE",
  "CLOUD",
  "CLOUD_CIRCLE",
  "CLOUD_DONE",
  "CLOUD_DOWNLOAD",
  "CLOUD_OFF",
  "CLOUD_QUEUE",
  "CLOUD_UPLOAD",
  "CODE",
  "COLLECTIONS",
  "COLLECTIONS_BOOKMARK",
  "COLORIZE",
  "COLOR_LENS",
  "COMMENT",
  "COMPARE_ARROWS",
  "COMPARE",
  "COMPUTER",
  "CONFIRMATION_NUMBER",
  "CONTACTS",
  "CONTACT_MAIL",
  "CONTACT_PHONE",
  "CONTENT_COPY",
  "CONTENT_CUT",
  "CONTENT_PASTE",
  "CONTROL_POINT",
  "CONTROL_POINT_DUPLICATE",
  "COPYRIGHT",
  "CREATE",
  "CREATE_NEW_FOLDER",
  "CREDIT_CARD",
  "CROP_16_9",
  "CROP_3_2",
  "CROP_5_4",
  "CROP_7_5",
  "CROP",
  "CROP_DIN",
  "CROP_FREE",
  "CROP_LANDSCAPE",
  "CROP_ORIGINAL",
  "CROP_PORTRAIT",
  "CROP_ROTATE",
  "CROP_SQUARE",
  "DASHBOARD",
  "DATE_RANGE",
  "DEHAZE",
  "DELETE",
  "DELETE_FOREVER",
  "DELETE_SWEEP",
  "DESCRIPTION",
  "DESKTOP_MAC",
  "DESKTOP_WINDOWS",
  "DETAILS",
  "DEVELOPER_BOARD",
  "DEVICES_OTHER",
  "DEVICE_HUB",
  "DIALER_SIP",
  "DIALPAD",
  "DIRECTIONS_BIKE",
  "DIRECTIONS",
  "DIRECTIONS_BOAT",
  "DIRECTIONS_BUS",
  "DIRECTIONS_CAR",
  "DIRECTIONS_RAILWAY",
  "DIRECTIONS_RUN",
  "DIRECTIONS_SUBWAY",
  "DIRECTIONS_TRANSIT",
  "DIRECTIONS_WALK",
  "DISC_FULL",
  "DNS",
  "DOCK",
  "DOMAIN",
  "DONE_ALL",
  "DONE",
  "DONUT_LARGE",
  "DONUT_SMALL",
  "DO_NOT_DISTURB_ALT",
  "DO_NOT_DISTURB",
  "DO_NOT_DISTURB_OFF",
  "DO_NOT_DISTURB_ON",
  "DRAFTS",
  "DRAG_HANDLE",
  "DRIVE_ETA",
  "EDIT",
  "EDIT_LOCATION",
  "EJECT",
  "EMAIL",
  "ENHANCED_ENCRYPTION",
  "EQUALIZER",
  "ERROR",
  "ERROR_OUTLINE",
  "EURO_SYMBOL",
  "EVENT_AVAILABLE",
  "EVENT",
  "EVENT_BUSY",
  "EVENT_NOTE",
  "EVENT_SEAT",
  "EV_STATION",
  "EXIT_TO_APP",
  "EXPAND_LESS",
  "EXPAND_MORE",
  "EXPLICIT",
  "EXPLORE",
  "EXPOSURE",
  "EXPOSURE_NEG_1",
  "EXPOSURE_NEG_2",
  "EXPOSURE_PLUS_1",
  "EXPOSURE_PLUS_2",
  "EXPOSURE_ZERO",
  "EXTENSION",
  "FACE",
  "FAST_FORWARD",
  "FAST_REWIND",
  "FAVORITE",
  "FAVORITE_BORDER",
  "FEATURED_PLAY_LIST",
  "FEATURED_VIDEO",
  "FEEDBACK",
  "FIBER_DVR",
  "FIBER_MANUAL_RECORD",
  "FIBER_NEW",
  "FIBER_PIN",
  "FIBER_SMART_RECORD",
  "FILE_DOWNLOAD",
  "FILE_UPLOAD",
  "FILTER_1",
  "FILTER_2",
  "FILTER_3",
  "FILTER_4",
  "FILTER_5",
  "FILTER_6",
  "FILTER_7",
  "FILTER_8",
  "FILTER_9",
  "FILTER_9_PLUS",
  "FILTER",
  "FILTER_B_AND_W",
  "FILTER_CENTER_FOCUS",
  "FILTER_DRAMA",
  "FILTER_FRAMES",
  "FILTER_HDR",
  "FILTER_LIST",
  "FILTER_NONE",
  "FILTER_TILT_SHIFT",
  "FILTER_VINTAGE",
  "FIND_IN_PAGE",
  "FIND_REPLACE",
  "FINGERPRINT",
  "FIRST_PAGE",
  "FITNESS_CENTER",
  "FLAG",
  "FLARE",
  "FLASH_AUTO",
  "FLASH_OFF",
  "FLASH_ON",
  "FLIGHT",
  "FLIGHT_LAND",
  "FLIGHT_TAKEOFF",
  "FLIP",
  "FLIP_TO_BACK",
  "FLIP_TO_FRONT",
  "FOLDER",
  "FOLDER_OPEN",
  "FOLDER_SHARED",
  "FOLDER_SPECIAL",
  "FONT_DOWNLOAD",
  "FORMAT_ALIGN_CENTER",
  "FORMAT_ALIGN_JUSTIFY",
  "FORMAT_ALIGN_LEFT",
  "FORMAT_ALIGN_RIGHT",
  "FORMAT_BOLD",
  "FORMAT_CLEAR",
  "FORMAT_COLOR_FILL",
  "FORMAT_COLOR_RESET",
  "FORMAT_COLOR_TEXT",
  "FORMAT_INDENT_DECREASE",
  "FORMAT_INDENT_INCREASE",
  "FORMAT_ITALBLACK_24DP",
  "FORMAT_LINE_SPACING",
  "FORMAT_LIST_BULLETED",
  "FORMAT_LIST_NUMBERED",
  "FORMAT_PAINT",
  "FORMAT_QUOTE",
  "FORMAT_SHAPES",
  "FORMAT_SIZE",
  "FORMAT_STRIKETHROUGH",
  "FORMAT_TEXTDIRECTION_L_TO_R",
  "FORMAT_TEXTDIRECTION_R_TO_L",
  "FORMAT_UNDERLINED",
  "FORUM",
  "FORWARD_10",
  "FORWARD_30",
  "FORWARD_5",
  "FORWARD",
  "FREE_BREAKFAST",
  "FULLSCREEN",
  "FULLSCREEN_EXIT",
  "FUNCTIONS",
  "GAMEPAD",
  "GAMES",
  "GAVEL",
  "GESTURE",
  "GET_APP",
  "GIF",
  "GOLF_COURSE",
  "GRADE",
  "GRADIENT",
  "GRAIN",
  "GRID_OFF",
  "GRID_ON",
  "GROUP_ADD",
  "GROUP",
  "GROUP_WORK",
  "G_TRANSLATE",
  "HDR_OFF",
  "HDR_ON",
  "HDR_STRONG",
  "HDR_WEAK",
  "HD",
  "HEADSET",
  "HEADSET_MBLACK_24DP",
  "HEALING",
  "HEARING",
  "HELP",
  "HELP_OUTLINE",
  "HIGHLIGHT",
  "HIGHLIGHT_OFF",
  "HIGH_QUALITY",
  "HISTORY",
  "HOME",
  "HOTEL",
  "HOT_TUB",
  "HOURGLASS_EMPTY",
  "HOURGLASS_FULL",
  "HTTPS",
  "HTTP",
  "IMAGE_ASPECT_RATIO",
  "IMAGE",
  "IMPORTANT_DEVICES",
  "IMPORT_CONTACTS",
  "IMPORT_EXPORT",
  "INBOX",
  "INDETERMINATE_CHECK_BOX",
  "INFO",
  "INFO_OUTLINE",
  "INPUT",
  "INSERT_CHART",
  "INSERT_COMMENT",
  "INSERT_DRIVE_FILE",
  "INSERT_EMOTICON",
  "INSERT_INVITATION",
  "INSERT_LINK",
  "INSERT_PHOTO",
  "INVERT_COLORS",
  "INVERT_COLORS_OFF",
  "ISO",
  "KEYBOARD_ARROW_DOWN",
  "KEYBOARD_ARROW_LEFT",
  "KEYBOARD_ARROW_RIGHT",
  "KEYBOARD_ARROW_UP",
  "KEYBOARD_BACKSPACE",
  "KEYBOARD",
  "KEYBOARD_CAPSLOCK",
  "KEYBOARD_HIDE",
  "KEYBOARD_RETURN",
  "KEYBOARD_TAB",
  "KEYBOARD_VOICE",
  "KITCHEN",
  "LABEL",
  "LABEL_OUTLINE",
  "LANDSCAPE",
  "LANGUAGE",
  "LAPTOP",
  "LAPTOP_CHROMEBOOK",
  "LAPTOP_MAC",
  "LAPTOP_WINDOWS",
  "LAST_PAGE",
  "LAUNCH",
  "LAYERS",
  "LAYERS_CLEAR",
  "LEAK_ADD",
  "LEAK_REMOVE",
  "LENS",
  "LIBRARY_ADD",
  "LIBRARY_BOOKS",
  "LIBRARY_MUSBLACK_24DP",
  "LIGHTBULB_OUTLINE",
  "LINEAR_SCALE",
  "LINE_STYLE",
  "LINE_WEIGHT",
  "LINKED_CAMERA",
  "LINK",
  "LIST",
  "LIVE_HELP",
  "LIVE_TV",
  "LOCAL_ACTIVITY",
  "LOCAL_AIRPORT",
  "LOCAL_ATM",
  "LOCAL_BAR",
  "LOCAL_CAFE",
  "LOCAL_CAR_WASH",
  "LOCAL_CONVENIENCE_STORE",
  "LOCAL_DINING",
  "LOCAL_DRINK",
  "LOCAL_FLORIST",
  "LOCAL_GAS_STATION",
  "LOCAL_GROCERY_STORE",
  "LOCAL_HOSPITAL",
  "LOCAL_HOTEL",
  "LOCAL_LAUNDRY_SERVICE",
  "LOCAL_LIBRARY",
  "LOCAL_MALL",
  "LOCAL_MOVIES",
  "LOCAL_OFFER",
  "LOCAL_PARKING",
  "LOCAL_PHARMACY",
  "LOCAL_PHONE",
  "LOCAL_PIZZA",
  "LOCAL_PLAY",
  "LOCAL_POST_OFFICE",
  "LOCAL_PRINTSHOP",
  "LOCAL_SEE",
  "LOCAL_SHIPPING",
  "LOCAL_TAXI",
  "LOCATION_CITY",
  "LOCATION_OFF",
  "LOCATION_ON",
  "LOCK",
  "LOCK_OPEN",
  "LOCK_OUTLINE",
  "LOOKS_3",
  "LOOKS_4",
  "LOOKS_5",
  "LOOKS_6",
  "LOOKS",
  "LOOKS_ONE",
  "LOOKS_TWO",
  "LOOP",
  "LOUPE",
  "LOW_PRIORITY",
  "LOYALTY",
  "MAIL",
  "MAIL_OUTLINE",
  "MAP",
  "MARKUNREAD",
  "MARKUNREAD_MAILBOX",
  "MEMORY",
  "MENU",
  "MERGE_TYPE",
  "MESSAGE",
  "MBLACK_24DP",
  "MNONE",
  "MOFF",
  "MMS",
  "MODE_COMMENT",
  "MODE_EDIT",
  "MONETIZATION_ON",
  "MONEY_OFF",
  "MONOCHROME_PHOTOS",
  "MOOD_BAD",
  "MOOD",
  "MORE",
  "MORE_HORIZ",
  "MORE_VERT",
  "MOTORCYCLE",
  "MOUSE",
  "MOVE_TO_INBOX",
  "MOVIE",
  "MOVIE_CREATION",
  "MOVIE_FILTER",
  "MULTILINE_CHART",
  "MUSNOTE",
  "MUSVIDEO",
  "MY_LOCATION",
  "NATURE",
  "NATURE_PEOPLE",
  "NAVIGATE_BEFORE",
  "NAVIGATE_NEXT",
  "NAVIGATION",
  "NEAR_ME",
  "NETWORK_CHECK",
  "NETWORK_LOCKED",
  "NEW_RELEASES",
  "NEXT_WEEK",
  "NOTE_ADD",
  "NOTE",
  "NOTIFICATIONS_ACTIVE",
  "NOTIFICATIONS",
  "NOTIFICATIONS_NONE",
  "NOTIFICATIONS_OFF",
  "NOTIFICATIONS_PAUSED",
  "NOT_INTERESTED",
  "NO_ENCRYPTION",
  "NO_SIM",
  "OFFLINE_PIN",
  "ONDEMAND_VIDEO",
  "OPACITY",
  "OPEN_IN_BROWSER",
  "OPEN_IN_NEW",
  "OPEN_WITH",
  "PAGES",
  "PAGEVIEW",
  "PALETTE",
  "PANORAMA",
  "PANORAMA_FISH_EYE",
  "PANORAMA_HORIZONTAL",
  "PANORAMA_VERTICAL",
  "PANORAMA_WIDE_ANGLE",
  "PAN_TOOL",
  "PARTY_MODE",
  "PAUSE",
  "PAUSE_CIRCLE_FILLED",
  "PAUSE_CIRCLE_OUTLINE",
  "PAYMENT",
  "PEOPLE",
  "PEOPLE_OUTLINE",
  "PERM_CAMERA_MBLACK_24DP",
  "PERM_CONTACT_CALENDAR",
  "PERM_DATA_SETTING",
  "PERM_DEVICE_INFORMATION",
  "PERM_IDENTITY",
  "PERM_MEDIA",
  "PERM_PHONE_MSG",
  "PERM_SCAN_WIFI",
  "PERSONAL_VIDEO",
  "PERSON_ADD",
  "PERSON",
  "PERSON_OUTLINE",
  "PERSON_PIN",
  "PERSON_PIN_CIRCLE",
  "PETS",
  "PHONELINK",
  "PHONELINK_ERASE",
  "PHONELINK_LOCK",
  "PHONELINK_OFF",
  "PHONELINK_RING",
  "PHONELINK_SETUP",
  "PHONE_ANDROID",
  "PHONE",
  "PHONE_BLUETOOTH_SPEAKER",
  "PHONE_FORWARDED",
  "PHONE_IN_TALK",
  "PHONE_IPHONE",
  "PHONE_LOCKED",
  "PHONE_MISSED",
  "PHONE_PAUSED",
  "PHOTO_ALBUM",
  "PHOTO",
  "PHOTO_CAMERA",
  "PHOTO_FILTER",
  "PHOTO_LIBRARY",
  "PHOTO_SIZE_SELECT_ACTUAL",
  "PHOTO_SIZE_SELECT_LARGE",
  "PHOTO_SIZE_SELECT_SMALL",
  "PICTURE_AS_PDF",
  "PICTURE_IN_PICTURE_ALT",
  "PICTURE_IN_PICTURE",
  "PIE_CHART",
  "PIE_CHART_OUTLINED",
  "PIN_DROP",
  "PLACE",
  "PLAYLIST_ADD",
  "PLAYLIST_ADD_CHECK",
  "PLAYLIST_PLAY",
  "PLAY_ARROW",
  "PLAY_CIRCLE_FILLED",
  "PLAY_CIRCLE_FILLED_WHITE",
  "PLAY_CIRCLE_OUTLINE",
  "PLAY_FOR_WORK",
  "PLUS_ONE",
  "POLL",
  "POLYMER",
  "POOL",
  "PORTABLE_WIFI_OFF",
  "PORTRAIT",
  "POWER",
  "POWER_INPUT",
  "POWER_SETTINGS_NEW",
  "PREGNANT_WOMAN",
  "PRESENT_TO_ALL",
  "PRINT",
  "PRIORITY_HIGH",
  "PUBLBLACK_24DP",
  "PUBLISH",
  "QUERY_BUILDER",
  "QUESTION_ANSWER",
  "QUEUE",
  "QUEUE_MUSBLACK_24DP",
  "QUEUE_PLAY_NEXT",
  "RADIO",
  "RADIO_BUTTON_CHECKED",
  "RADIO_BUTTON_UNCHECKED",
  "RATE_REVIEW",
  "RECEIPT",
  "RECENT_ACTORS",
  "RECORD_VOICE_OVER",
  "REDEEM",
  "REDO",
  "REFRESH",
  "REMOVE",
  "REMOVE_CIRCLE",
  "REMOVE_CIRCLE_OUTLINE",
  "REMOVE_FROM_QUEUE",
  "REMOVE_RED_EYE",
  "REMOVE_SHOPPING_CART",
  "REORDER",
  "REPEAT",
  "REPEAT_ONE",
  "REPLAY_10",
  "REPLAY_30",
  "REPLAY_5",
  "REPLAY",
  "REPLY_ALL",
  "REPLY",
  "REPORT",
  "REPORT_PROBLEM",
  "RESTAURANT",
  "RESTAURANT_MENU",
  "RESTORE",
  "RESTORE_PAGE",
  "RING_VOLUME",
  "ROOM",
  "ROOM_SERVICE",
  "ROTATE_90_DEGREES_CCW",
  "ROTATE_LEFT",
  "ROTATE_RIGHT",
  "ROUNDED_CORNER",
  "ROUTER",
  "ROWING",
  "RSS_FEED",
  "RV_HOOKUP",
  "SATELLITE",
  "SAVE",
  "SCANNER",
  "SCHEDULE",
  "SCHOOL",
  "SCREEN_SHARE",
  "SD_CARD",
  "SEARCH",
  "SECURITY",
  "SELECT_ALL",
  "SEND",
  "SENTIMENT_DISSATISFIED",
  "SENTIMENT_NEUTRAL",
  "SENTIMENT_SATISFIED",
  "SENTIMENT_VERY_DISSATISFIED",
  "SENTIMENT_VERY_SATISFIED",
  "SETTINGS_APPLICATIONS",
  "SETTINGS_BACKUP_RESTORE",
  "SETTINGS",
  "SETTINGS_BLUETOOTH",
  "SETTINGS_BRIGHTNESS",
  "SETTINGS_CELL",
  "SETTINGS_ETHERNET",
  "SETTINGS_INPUT_ANTENNA",
  "SETTINGS_INPUT_COMPONENT",
  "SETTINGS_INPUT_COMPOSITE",
  "SETTINGS_INPUT_HDMI",
  "SETTINGS_INPUT_SVIDEO",
  "SETTINGS_OVERSCAN",
  "SETTINGS_PHONE",
  "SETTINGS_POWER",
  "SETTINGS_REMOTE",
  "SETTINGS_VOICE",
  "SHARE",
  "SHOPPING_BASKET",
  "SHOPPING_CART",
  "SHOP",
  "SHOP_TWO",
  "SHORT_TEXT",
  "SHOW_CHART",
  "SHUFFLE",
  "SIM_CARD_ALERT",
  "SIM_CARD",
  "SKIP_NEXT",
  "SKIP_PREVIOUS",
  "SLIDESHOW",
  "SLOW_MOTION_VIDEO",
  "SMARTPHONE",
  "SMOKE_FREE",
  "SMOKING_ROOMS",
  "SMS",
  "SMS_FAILED",
  "SNOOZE",
  "SORT",
  "SORT_BY_ALPHA",
  "SPACE_BAR",
  "SPA",
  "SPEAKER",
  "SPEAKER_GROUP",
  "SPEAKER_NOTES",
  "SPEAKER_NOTES_OFF",
  "SPEAKER_PHONE",
  "SPELLCHECK",
  "STARS",
  "STAR",
  "STAR_BORDER",
  "STAR_HALF",
  "STAY_CURRENT_LANDSCAPE",
  "STAY_CURRENT_PORTRAIT",
  "STAY_PRIMARY_LANDSCAPE",
  "STAY_PRIMARY_PORTRAIT",
  "STOP",
  "STOP_SCREEN_SHARE",
  "STORE",
  "STORE_MALL_DIRECTORY",
  "STRAIGHTEN",
  "STREETVIEW",
  "STRIKETHROUGH_S",
  "STYLE",
  "SUBDIRECTORY_ARROW_LEFT",
  "SUBDIRECTORY_ARROW_RIGHT",
  "SUBJECT",
  "SUBSCRIPTIONS",
  "SUBTITLES",
  "SUBWAY",
  "SUPERVISOR_ACCOUNT",
  "SURROUND_SOUND",
  "SWAP_CALLS",
  "SWAP_HORIZ",
  "SWAP_VERTICAL_CIRCLE",
  "SWAP_VERT",
  "SWITCH_CAMERA",
  "SWITCH_VIDEO",
  "SYNC",
  "SYNC_DISABLED",
  "SYNC_PROBLEM",
  "SYSTEM_UPDATE_ALT",
  "SYSTEM_UPDATE",
  "TABLET_ANDROID",
  "TABLET",
  "TABLET_MAC",
  "TAB",
  "TAB_UNSELECTED",
  "TAG_FACES",
  "TAP_AND_PLAY",
  "TERRAIN",
  "TEXTSMS",
  "TEXTURE",
  "TEXT_FIELDS",
  "TEXT_FORMAT",
  "THEATERS",
  "THUMBS_UP_DOWN",
  "THUMB_DOWN",
  "THUMB_UP",
  "TIMELAPSE",
  "TIMELINE",
  "TIMER_10",
  "TIMER_3",
  "TIMER",
  "TIMER_OFF",
  "TIME_TO_LEAVE",
  "TITLE",
  "TOC",
  "TODAY",
  "TOLL",
  "TONALITY",
  "TOUCH_APP",
  "TOYS",
  "TRACK_CHANGES",
  "TRAFFBLACK_24DP",
  "TRAIN",
  "TRAM",
  "TRANSFER_WITHIN_A_STATION",
  "TRANSFORM",
  "TRANSLATE",
  "TRENDING_DOWN",
  "TRENDING_FLAT",
  "TRENDING_UP",
  "TUNE",
  "TURNED_IN",
  "TURNED_IN_NOT",
  "TV",
  "UNARCHIVE",
  "UNDO",
  "UNFOLD_LESS",
  "UNFOLD_MORE",
  "UPDATE",
  "VERIFIED_USER",
  "VERTICAL_ALIGN_BOTTOM",
  "VERTICAL_ALIGN_CENTER",
  "VERTICAL_ALIGN_TOP",
  "VIBRATION",
  "VIDEOCAM",
  "VIDEOCAM_OFF",
  "VIDEOGAME_ASSET",
  "VIDEO_CALL",
  "VIDEO_LABEL",
  "VIDEO_LIBRARY",
  "VIEW_AGENDA",
  "VIEW_ARRAY",
  "VIEW_CAROUSEL",
  "VIEW_COLUMN",
  "VIEW_COMFY",
  "VIEW_COMPACT",
  "VIEW_DAY",
  "VIEW_HEADLINE",
  "VIEW_LIST",
  "VIEW_MODULE",
  "VIEW_QUILT",
  "VIEW_STREAM",
  "VIEW_WEEK",
  "VIGNETTE",
  "VISIBILITY",
  "VISIBILITY_OFF",
  "VOICEMAIL",
  "VOICE_CHAT",
  "VOLUME_DOWN",
  "VOLUME_MUTE",
  "VOLUME_OFF",
  "VOLUME_UP",
  "VPN_KEY",
  "VPN_LOCK",
  "WARNING",
  "WATCH",
  "WATCH_LATER",
  "WB_AUTO",
  "WB_CLOUDY",
  "WB_INCANDESCENT",
  "WB_IRIDESCENT",
  "WB_SUNNY",
  "WC",
  "WEB_ASSET",
  "WEB",
  "WEEKEND",
  "WHATSHOT",
  "WIFI",
  "WORK",
  "WRAP_TEXT",
  "YOUTUBE_SEARCHED_FOR",
  "ZOOM_IN",
  "ZOOM_OUT",
  "ZOOM_OUT_MAP"
};


void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");

  if (strcmp(topic, "cmnd/LED_Display/Colours") == 0)
  {
    Serial.println("Updating Colours");
    StaticJsonDocument<512> root;
    deserializeJson(root, payload, length);

    JsonObject LED1 = root["LED1"];
    if (LED1)
    {
      if (!root["LED1"]["H"].isNull())
      {
        LED1_H = map(LED1["H"], 0, 360, 0, 255); // Hue
      }
      if (!root["LED1"]["S"].isNull())
      {
        LED1_S = map(LED1["S"], 0, 100, 0, 255); // Sat
      }
      if (!root["LED1"]["V"].isNull())
      {
        LED1_V = map(LED1["V"], 0, 100, 0, 255); // Val
      }
      if (!root["LED1"]["FLASH"].isNull())
      {
        if (root["LED1"]["FLASH"].as<int>() == 1) {
          FlashArray[4] = true;
        }
        else if (root["LED1"]["FLASH"].as<int>() == 0) {
          FlashArray[4] = false;
        }
      }
    }

    JsonObject LED2 = root["LED2"];
    if (LED2)
    {
      if (!root["LED2"]["H"].isNull())
      {
        LED2_H = map(LED2["H"], 0, 360, 0, 255); // Hue
      }
      if (!root["LED2"]["S"].isNull())
      {
        LED2_S = map(LED2["S"], 0, 100, 0, 255); // Sat
      }
      if (!root["LED2"]["V"].isNull())
      {
        LED2_V = map(LED2["V"], 0, 100, 0, 255); // Val
      }
      if (!root["LED2"]["FLASH"].isNull())
      {
        if (root["LED2"]["FLASH"].as<int>() == 1) {
          FlashArray[3] = true;
        }
        else if (root["LED2"]["FLASH"].as<int>() == 0) {
          FlashArray[3] = false;
        }
      }
    }

    JsonObject LED3 = root["LED3"];
    if (LED3)
    {
      if (!root["LED3"]["H"].isNull())
      {
        LED3_H = map(LED3["H"], 0, 360, 0, 255); // Hue
      }
      if (!root["LED3"]["S"].isNull())
      {
        LED3_S = map(LED3["S"], 0, 100, 0, 255); // Sat
      }
      if (!root["LED3"]["V"].isNull())
      {
        LED3_V = map(LED3["V"], 0, 100, 0, 255); // Val
      }
      if (!root["LED3"]["FLASH"].isNull())
      {
        if (root["LED3"]["FLASH"].as<int>() == 1) {
          FlashArray[2] = true;
        }
        else if (root["LED3"]["FLASH"].as<int>() == 0) {
          FlashArray[2] = false;
        }
      }
    }

    JsonObject LED4 = root["LED4"];
    if (LED4)
    {
      if (!root["LED4"]["H"].isNull())
      {
        LED4_H = map(LED4["H"], 0, 360, 0, 255); // Hue
      }
      if (!root["LED4"]["S"].isNull())
      {
        LED4_S = map(LED4["S"], 0, 100, 0, 255); // Sat
      }
      if (!root["LED4"]["V"].isNull())
      {
        LED4_V = map(LED4["V"], 0, 100, 0, 255); // Val
      }
      if (!root["LED4"]["FLASH"].isNull())
      {
        if (root["LED4"]["FLASH"].as<int>() == 1) {
          FlashArray[1] = true;
        }
        else if (root["LED4"]["FLASH"].as<int>() == 0) {
          FlashArray[1] = false;
        }
      }
    }

    JsonObject LED5 = root["LED5"];
    if (LED5)
    {
      if (!root["LED5"]["H"].isNull())
      {
        LED5_H = map(LED5["H"], 0, 360, 0, 255); // Hue
      }
      if (!root["LED5"]["S"].isNull())
      {
        LED5_S = map(LED5["S"], 0, 100, 0, 255); // Sat
      }
      if (!root["LED5"]["V"].isNull())
      {
        LED5_V = map(LED5["V"], 0, 100, 0, 255); // Val
      }
      if (!root["LED5"]["FLASH"].isNull())
      {
        if (root["LED5"]["FLASH"].as<int>() == 1) {
          FlashArray[0] = true;
        }
        else if (root["LED5"]["FLASH"].as<int>() == 0) {
          FlashArray[0] = false;
        }
      }
    }

    Serial.println("LED1: " + String(LED1_H) + "\t" + String(LED1_S) + "\t" + String(LED1_V) + "\tFLASH: " + String(FlashArray[0]));
    Serial.println("LED2: " + String(LED2_H) + "\t" + String(LED2_S) + "\t" + String(LED2_V) + "\tFLASH: " + String(FlashArray[1]));
    Serial.println("LED3: " + String(LED3_H) + "\t" + String(LED3_S) + "\t" + String(LED3_V) + "\tFLASH: " + String(FlashArray[2]));
    Serial.println("LED4: " + String(LED4_H) + "\t" + String(LED4_S) + "\t" + String(LED4_V) + "\tFLASH: " + String(FlashArray[3]));
    Serial.println("LED5: " + String(LED5_H) + "\t" + String(LED5_S) + "\t" + String(LED5_V) + "\tFLASH: " + String(FlashArray[4]));
  }
  if (strcmp(topic, "cmnd/LED_Display/Display") == 0)
  {
    Serial.println("Updating Display");
    DynamicJsonDocument doc(512);
    deserializeJson(doc, payload, length);

    JsonObject PAGE1 = doc["PAGE1"];
    if (PAGE1)
    {
      if (!doc["PAGE1"]["TITLE"].isNull())
      {
        const char* PAGE1_TITLE = doc["PAGE1"]["TITLE"];
        Page1Title = String(PAGE1_TITLE);
      }
      if (!doc["PAGE1"]["SUBTEXT"].isNull())
      {
        const char* PAGE1_SUBTEXT = doc["PAGE1"]["SUBTEXT"];
        Page1Subtext = String(PAGE1_SUBTEXT);
      }
      if (!doc["PAGE1"]["ICON"].isNull())
      {
        const char* PAGE1_ICON = doc["PAGE1"]["ICON"];
        Page1Icon = String(PAGE1_ICON);
      }
    }

    JsonObject PAGE2 = doc["PAGE2"];
    if (PAGE2)
    {
      if (!doc["PAGE2"]["TITLE"].isNull())
      {
        const char* PAGE2_TITLE = doc["PAGE2"]["TITLE"];
        Page2Title = String(PAGE2_TITLE);
      }
      if (!doc["PAGE2"]["SUBTEXT"].isNull())
      {
        const char* PAGE2_SUBTEXT = doc["PAGE2"]["SUBTEXT"];
        Page2Subtext = String(PAGE2_SUBTEXT);
      }
      if (!doc["PAGE2"]["ICON"].isNull())
      {
        const char* PAGE2_ICON = doc["PAGE2"]["ICON"];
        Page2Icon = String(PAGE2_ICON);
      }
    }

    JsonObject PAGE3 = doc["PAGE3"];
    if (PAGE3)
    {
      if (!doc["PAGE3"]["TITLE"].isNull())
      {
        const char* PAGE3_TITLE = doc["PAGE3"]["TITLE"];
        Page3Title = String(PAGE3_TITLE);
      }
      if (!doc["PAGE3"]["SUBTEXT"].isNull())
      {
        const char* PAGE3_SUBTEXT = doc["PAGE3"]["SUBTEXT"];
        Page3Subtext = String(PAGE3_SUBTEXT);
      }
      if (!doc["PAGE3"]["ICON"].isNull())
      {
        const char* PAGE3_ICON = doc["PAGE3"]["ICON"];
        Page3Icon = String(PAGE3_ICON);
      }
    }

    JsonObject PAGE4 = doc["PAGE4"];
    if (PAGE4)
    {
      if (!doc["PAGE4"]["TITLE"].isNull())
      {
        const char* PAGE4_TITLE = doc["PAGE4"]["TITLE"];
        Page4Title = String(PAGE4_TITLE);
      }
      if (!doc["PAGE4"]["SUBTEXT"].isNull())
      {
        const char* PAGE4_SUBTEXT = doc["PAGE4"]["SUBTEXT"];
        Page4Subtext = String(PAGE4_SUBTEXT);
      }
      if (!doc["PAGE4"]["ICON"].isNull())
      {
        const char* PAGE4_ICON = doc["PAGE4"]["ICON"];
        Page4Icon = String(PAGE4_ICON);
      }
    }

    JsonObject PAGE5 = doc["PAGE5"];
    if (PAGE5)
    {
      if (!doc["PAGE5"]["TITLE"].isNull())
      {
        const char* PAGE5_TITLE = doc["PAGE5"]["TITLE"];
        Page5Title = String(PAGE5_TITLE);
      }
      if (!doc["PAGE5"]["SUBTEXT"].isNull())
      {
        const char* PAGE5_SUBTEXT = doc["PAGE5"]["SUBTEXT"];
        Page5Subtext = String(PAGE5_SUBTEXT);
      }
      if (!doc["PAGE5"]["ICON"].isNull())
      {
        const char* PAGE5_ICON = doc["PAGE5"]["ICON"];
        Page5Icon = String(PAGE5_ICON);
      }
    }

    Serial.println("PAGE1 Title: " + String(Page1Title) + "\tPAGE1 Subtext: " + String(Page1Subtext) + "\tPAGE1 Icon: " + String(Page1Icon));
    Serial.println("PAGE2 Title: " + String(Page2Title) + "\tPAGE2 Subtext: " + String(Page2Subtext) + "\tPAGE2 Icon: " + String(Page2Icon));
    Serial.println("PAGE3 Title: " + String(Page3Title) + "\tPAGE3 Subtext: " + String(Page3Subtext) + "\tPAGE3 Icon: " + String(Page3Icon));
    Serial.println("PAGE4 Title: " + String(Page4Title) + "\tPAGE4 Subtext: " + String(Page4Subtext) + "\tPAGE4 Icon: " + String(Page4Icon));
    Serial.println("PAGE5 Title: " + String(Page5Title) + "\tPAGE5 Subtext: " + String(Page5Subtext) + "\tPAGE5 Icon: " + String(Page5Icon));
  }
}

void reconnect() {
  // Loop until we're reconnected
  //while (!client.connected()) {
  Serial.print("Attempting MQTT connection...");
  // Create a random client ID
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  // Attempt to connect
  if (client.connect(clientId.c_str(), mqtt_user, mqtt_password, will_topic, will_QoS, will_retain, will_message)) {
    Serial.println("connected");
    client.publish(will_topic, lwt_msg, will_retain);
    client.subscribe("cmnd/LED_Display/Colours");
    client.subscribe("cmnd/LED_Display/Display");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
  //}
}

void drawProgress(OLEDDisplay * display, int x, int y, String label, int percentage, String IconType) {
  display->clear();
  drawScreen(display, x, y, "", "", IconType);
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(120, 10, label);
  display->drawProgressBar(10, 50, 108, 10, percentage);
  display->display();
}

int FindIconIndex(String IconName) {
  for (int i = 0; i < sizeof(IconArray); i++) {
    if (IconName == IconArray[i]) {
      return i;
    }
  }
  return 0; // -1 means "not found"
}

void drawScreen(OLEDDisplay * display, int x, int y, String TitleText, String SubText, String IconType) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 55, y + 5, TitleText);
  //display->setFont(Crushed_Plain_36);
  display->drawStringMaxWidth(x + 55, y + 20, 70, SubText);
  //display->drawString(x + 60, y + 15, SubText);
  int IconInt = FindIconIndex(IconType);
  //Serial.print("Icon Index: ");
  //Serial.println(IconInt);

  switch (IconInt)
  {
    case  1 :
      break;

    case  2 :
      display->drawXbm(x, y, 48, 48, IC_3D_ROTATION);
      break;

    case  3 :
      display->drawXbm(x, y, 48, 48, IC_ACCESSIBILITY);
      break;

    case  4 :
      display->drawXbm(x, y, 48, 48, IC_ACCESSIBLE);
      break;

    case  5 :
      display->drawXbm(x, y, 48, 48, IC_ACCOUNT_BALANCE);
      break;

    case  6 :
      display->drawXbm(x, y, 48, 48, IC_ACCOUNT_BALANCE_WALLET);
      break;

    case  7 :
      display->drawXbm(x, y, 48, 48, IC_ACCOUNT_BOX);
      break;

    case  8 :
      display->drawXbm(x, y, 48, 48, IC_ACCOUNT_CIRCLE);
      break;

    case  9 :
      display->drawXbm(x, y, 48, 48, IC_AC_UNIT);
      break;

    case  10 :
      display->drawXbm(x, y, 48, 48, IC_ADB);
      break;

    case  11 :
      display->drawXbm(x, y, 48, 48, IC_ADD_ALERT);
      break;

    case  12 :
      display->drawXbm(x, y, 48, 48, IC_ADD_A_PHOTO);
      break;

    case  13 :
      display->drawXbm(x, y, 48, 48, IC_ADD);
      break;

    case  14 :
      display->drawXbm(x, y, 48, 48, IC_ADD_BOX);
      break;

    case  15 :
      display->drawXbm(x, y, 48, 48, IC_ADD_CIRCLE);
      break;

    case  16 :
      display->drawXbm(x, y, 48, 48, IC_ADD_CIRCLE_OUTLINE);
      break;

    case  17 :
      display->drawXbm(x, y, 48, 48, IC_ADD_LOCATION);
      break;

    case  18 :
      display->drawXbm(x, y, 48, 48, IC_ADD_SHOPPING_CART);
      break;

    case  19 :
      display->drawXbm(x, y, 48, 48, IC_ADD_TO_PHOTOS);
      break;

    case  20 :
      display->drawXbm(x, y, 48, 48, IC_ADD_TO_QUEUE);
      break;

    case  21 :
      display->drawXbm(x, y, 48, 48, IC_ADJUST);
      break;

    case  22 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_FLAT_ANGLED);
      break;

    case  23 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_FLAT);
      break;

    case  24 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_INDIVIDUAL_SUITE);
      break;

    case  25 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_LEGROOM_EXTRA);
      break;

    case  26 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_LEGROOM_NORMAL);
      break;

    case  27 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_LEGROOM_REDUCED);
      break;

    case  28 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_RECLINE_EXTRA);
      break;

    case  29 :
      display->drawXbm(x, y, 48, 48, IC_AIRLINE_SEAT_RECLINE_NORMAL);
      break;

    case  30 :
      display->drawXbm(x, y, 48, 48, IC_AIRPLAY);
      break;

    case  31 :
      display->drawXbm(x, y, 48, 48, IC_AIRPORT_SHUTTLE);
      break;

    case  32 :
      display->drawXbm(x, y, 48, 48, IC_ALARM_ADD);
      break;

    case  33 :
      display->drawXbm(x, y, 48, 48, IC_ALARM);
      break;

    case  34 :
      display->drawXbm(x, y, 48, 48, IC_ALARM_OFF);
      break;

    case  35 :
      display->drawXbm(x, y, 48, 48, IC_ALARM_ON);
      break;

    case  36 :
      display->drawXbm(x, y, 48, 48, IC_ALBUM);
      break;

    case  37 :
      display->drawXbm(x, y, 48, 48, IC_ALL_INCLUSIVE);
      break;

    case  38 :
      display->drawXbm(x, y, 48, 48, IC_ALL_OUT);
      break;

    case  39 :
      display->drawXbm(x, y, 48, 48, IC_ANDROID);
      break;

    case  40 :
      display->drawXbm(x, y, 48, 48, IC_ANNOUNCEMENT);
      break;

    case  41 :
      display->drawXbm(x, y, 48, 48, IC_APPS);
      break;

    case  42 :
      display->drawXbm(x, y, 48, 48, IC_ARCHIVE);
      break;

    case  43 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_BACK);
      break;

    case  44 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_DOWNWARD);
      break;

    case  45 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_DROP_DOWN);
      break;

    case  46 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_DROP_DOWN_CIRCLE);
      break;

    case  47 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_DROP_UP);
      break;

    case  48 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_FORWARD);
      break;

    case  49 :
      display->drawXbm(x, y, 48, 48, IC_ARROW_UPWARD);
      break;

    case  50 :
      display->drawXbm(x, y, 48, 48, IC_ART_TRACK);
      break;

    case  51 :
      display->drawXbm(x, y, 48, 48, IC_ASPECT_RATIO);
      break;

    case  52 :
      display->drawXbm(x, y, 48, 48, IC_ASSESSMENT);
      break;

    case  53 :
      display->drawXbm(x, y, 48, 48, IC_ASSIGNMENT);
      break;

    case  54 :
      display->drawXbm(x, y, 48, 48, IC_ASSIGNMENT_IND);
      break;

    case  55 :
      display->drawXbm(x, y, 48, 48, IC_ASSIGNMENT_LATE);
      break;

    case  56 :
      display->drawXbm(x, y, 48, 48, IC_ASSIGNMENT_RETURNED);
      break;

    case  57 :
      display->drawXbm(x, y, 48, 48, IC_ASSIGNMENT_RETURN);
      break;

    case  58 :
      display->drawXbm(x, y, 48, 48, IC_ASSIGNMENT_TURNED_IN);
      break;

    case  59 :
      display->drawXbm(x, y, 48, 48, IC_ASSISTANT);
      break;

    case  60 :
      display->drawXbm(x, y, 48, 48, IC_ASSISTANT_PHOTO);
      break;

    case  61 :
      display->drawXbm(x, y, 48, 48, IC_ATTACHMENT);
      break;

    case  62 :
      display->drawXbm(x, y, 48, 48, IC_ATTACH_FILE);
      break;

    case  63 :
      display->drawXbm(x, y, 48, 48, IC_ATTACH_MONEY);
      break;

    case  64 :
      display->drawXbm(x, y, 48, 48, IC_AUDIOTRACK);
      break;

    case  65 :
      display->drawXbm(x, y, 48, 48, IC_AUTORENEW);
      break;

    case  66 :
      display->drawXbm(x, y, 48, 48, IC_AV_TIMER);
      break;

    case  67 :
      display->drawXbm(x, y, 48, 48, IC_BACKSPACE);
      break;

    case  68 :
      display->drawXbm(x, y, 48, 48, IC_BACKUP);
      break;

    case  69 :
      display->drawXbm(x, y, 48, 48, IC_BEACH_ACCESS);
      break;

    case  70 :
      display->drawXbm(x, y, 48, 48, IC_BEENHERE);
      break;

    case  71 :
      display->drawXbm(x, y, 48, 48, IC_BLOCK);
      break;

    case  72 :
      display->drawXbm(x, y, 48, 48, IC_BLUETOOTH_AUDIO);
      break;

    case  73 :
      display->drawXbm(x, y, 48, 48, IC_BLUR_CIRCULAR);
      break;

    case  74 :
      display->drawXbm(x, y, 48, 48, IC_BLUR_LINEAR);
      break;

    case  75 :
      display->drawXbm(x, y, 48, 48, IC_BLUR_OFF);
      break;

    case  76 :
      display->drawXbm(x, y, 48, 48, IC_BLUR_ON);
      break;

    case  77 :
      display->drawXbm(x, y, 48, 48, IC_BOOKMARK);
      break;

    case  78 :
      display->drawXbm(x, y, 48, 48, IC_BOOKMARK_BORDER);
      break;

    case  79 :
      display->drawXbm(x, y, 48, 48, IC_BOOK);
      break;

    case  80 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_ALL);
      break;

    case  81 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_BOTTOM);
      break;

    case  82 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_CLEAR);
      break;

    case  83 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_COLOR);
      break;

    case  84 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_HORIZONTAL);
      break;

    case  85 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_INNER);
      break;

    case  86 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_LEFT);
      break;

    case  87 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_OUTER);
      break;

    case  88 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_RIGHT);
      break;

    case  89 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_STYLE);
      break;

    case  90 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_TOP);
      break;

    case  91 :
      display->drawXbm(x, y, 48, 48, IC_BORDER_VERTICAL);
      break;

    case  92 :
      display->drawXbm(x, y, 48, 48, IC_BRANDING_WATERMARK);
      break;

    case  93 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_1);
      break;

    case  94 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_2);
      break;

    case  95 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_3);
      break;

    case  96 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_4);
      break;

    case  97 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_5);
      break;

    case  98 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_6);
      break;

    case  99 :
      display->drawXbm(x, y, 48, 48, IC_BRIGHTNESS_7);
      break;

    case  100 :
      display->drawXbm(x, y, 48, 48, IC_BROKEN_IMAGE);
      break;

    case  101 :
      display->drawXbm(x, y, 48, 48, IC_BRUSH);
      break;

    case  102 :
      display->drawXbm(x, y, 48, 48, IC_BUBBLE_CHART);
      break;

    case  103 :
      display->drawXbm(x, y, 48, 48, IC_BUG_REPORT);
      break;

    case  104 :
      display->drawXbm(x, y, 48, 48, IC_BUILD);
      break;

    case  105 :
      display->drawXbm(x, y, 48, 48, IC_BURST_MODE);
      break;

    case  106 :
      display->drawXbm(x, y, 48, 48, IC_BUSINESS);
      break;

    case  107 :
      display->drawXbm(x, y, 48, 48, IC_BUSINESS_CENTER);
      break;

    case  108 :
      display->drawXbm(x, y, 48, 48, IC_CACHED);
      break;

    case  109 :
      display->drawXbm(x, y, 48, 48, IC_CAKE);
      break;

    case  110 :
      display->drawXbm(x, y, 48, 48, IC_CALL);
      break;

    case  111 :
      display->drawXbm(x, y, 48, 48, IC_CALL_END);
      break;

    case  112 :
      display->drawXbm(x, y, 48, 48, IC_CALL_MADE);
      break;

    case  113 :
      display->drawXbm(x, y, 48, 48, IC_CALL_MERGE);
      break;

    case  114 :
      display->drawXbm(x, y, 48, 48, IC_CALL_MISSED);
      break;

    case  115 :
      display->drawXbm(x, y, 48, 48, IC_CALL_MISSED_OUTGOING);
      break;

    case  116 :
      display->drawXbm(x, y, 48, 48, IC_CALL_RECEIVED);
      break;

    case  117 :
      display->drawXbm(x, y, 48, 48, IC_CALL_SPLIT);
      break;

    case  118 :
      display->drawXbm(x, y, 48, 48, IC_CALL_TO_ACTION);
      break;

    case  119 :
      display->drawXbm(x, y, 48, 48, IC_CAMERA_ALT);
      break;

    case  120 :
      display->drawXbm(x, y, 48, 48, IC_CAMERA);
      break;

    case  121 :
      display->drawXbm(x, y, 48, 48, IC_CAMERA_ENHANCE);
      break;

    case  122 :
      display->drawXbm(x, y, 48, 48, IC_CAMERA_FRONT);
      break;

    case  123 :
      display->drawXbm(x, y, 48, 48, IC_CAMERA_REAR);
      break;

    case  124 :
      display->drawXbm(x, y, 48, 48, IC_CAMERA_ROLL);
      break;

    case  125 :
      display->drawXbm(x, y, 48, 48, IC_CANCEL);
      break;

    case  126 :
      display->drawXbm(x, y, 48, 48, IC_CARD_GIFTCARD);
      break;

    case  127 :
      display->drawXbm(x, y, 48, 48, IC_CARD_MEMBERSHIP);
      break;

    case  128 :
      display->drawXbm(x, y, 48, 48, IC_CARD_TRAVEL);
      break;

    case  129 :
      display->drawXbm(x, y, 48, 48, IC_CASINO);
      break;

    case  130 :
      display->drawXbm(x, y, 48, 48, IC_CAST);
      break;

    case  131 :
      display->drawXbm(x, y, 48, 48, IC_CAST_CONNECTED);
      break;

    case  132 :
      display->drawXbm(x, y, 48, 48, IC_CENTER_FOCUS_STRONG);
      break;

    case  133 :
      display->drawXbm(x, y, 48, 48, IC_CENTER_FOCUS_WEAK);
      break;

    case  134 :
      display->drawXbm(x, y, 48, 48, IC_CHANGE_HISTORY);
      break;

    case  135 :
      display->drawXbm(x, y, 48, 48, IC_CHAT);
      break;

    case  136 :
      display->drawXbm(x, y, 48, 48, IC_CHAT_BUBBLE);
      break;

    case  137 :
      display->drawXbm(x, y, 48, 48, IC_CHAT_BUBBLE_OUTLINE);
      break;

    case  138 :
      display->drawXbm(x, y, 48, 48, IC_CHECK);
      break;

    case  139 :
      display->drawXbm(x, y, 48, 48, IC_CHECK_BOX);
      break;

    case  140 :
      display->drawXbm(x, y, 48, 48, IC_CHECK_BOX_OUTLINE_BLANK);
      break;

    case  141 :
      display->drawXbm(x, y, 48, 48, IC_CHECK_CIRCLE);
      break;

    case  142 :
      display->drawXbm(x, y, 48, 48, IC_CHEVRON_LEFT);
      break;

    case  143 :
      display->drawXbm(x, y, 48, 48, IC_CHEVRON_RIGHT);
      break;

    case  144 :
      display->drawXbm(x, y, 48, 48, IC_CHILD_CARE);
      break;

    case  145 :
      display->drawXbm(x, y, 48, 48, IC_CHILD_FRIENDLY);
      break;

    case  146 :
      display->drawXbm(x, y, 48, 48, IC_CHROME_READER_MODE);
      break;

    case  147 :
      display->drawXbm(x, y, 48, 48, IC_CLASS);
      break;

    case  148 :
      display->drawXbm(x, y, 48, 48, IC_CLEAR_ALL);
      break;

    case  149 :
      display->drawXbm(x, y, 48, 48, IC_CLEAR);
      break;

    case  150 :
      display->drawXbm(x, y, 48, 48, IC_CLOSED_CAPTION);
      break;

    case  151 :
      display->drawXbm(x, y, 48, 48, IC_CLOSE);
      break;

    case  152 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD);
      break;

    case  153 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD_CIRCLE);
      break;

    case  154 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD_DONE);
      break;

    case  155 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD_DOWNLOAD);
      break;

    case  156 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD_OFF);
      break;

    case  157 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD_QUEUE);
      break;

    case  158 :
      display->drawXbm(x, y, 48, 48, IC_CLOUD_UPLOAD);
      break;

    case  159 :
      display->drawXbm(x, y, 48, 48, IC_CODE);
      break;

    case  160 :
      display->drawXbm(x, y, 48, 48, IC_COLLECTIONS);
      break;

    case  161 :
      display->drawXbm(x, y, 48, 48, IC_COLLECTIONS_BOOKMARK);
      break;

    case  162 :
      display->drawXbm(x, y, 48, 48, IC_COLORIZE);
      break;

    case  163 :
      display->drawXbm(x, y, 48, 48, IC_COLOR_LENS);
      break;

    case  164 :
      display->drawXbm(x, y, 48, 48, IC_COMMENT);
      break;

    case  165 :
      display->drawXbm(x, y, 48, 48, IC_COMPARE_ARROWS);
      break;

    case  166 :
      display->drawXbm(x, y, 48, 48, IC_COMPARE);
      break;

    case  167 :
      display->drawXbm(x, y, 48, 48, IC_COMPUTER);
      break;

    case  168 :
      display->drawXbm(x, y, 48, 48, IC_CONFIRMATION_NUMBER);
      break;

    case  169 :
      display->drawXbm(x, y, 48, 48, IC_CONTACTS);
      break;

    case  170 :
      display->drawXbm(x, y, 48, 48, IC_CONTACT_MAIL);
      break;

    case  171 :
      display->drawXbm(x, y, 48, 48, IC_CONTACT_PHONE);
      break;

    case  172 :
      display->drawXbm(x, y, 48, 48, IC_CONTENT_COPY);
      break;

    case  173 :
      display->drawXbm(x, y, 48, 48, IC_CONTENT_CUT);
      break;

    case  174 :
      display->drawXbm(x, y, 48, 48, IC_CONTENT_PASTE);
      break;

    case  175 :
      display->drawXbm(x, y, 48, 48, IC_CONTROL_POINT);
      break;

    case  176 :
      display->drawXbm(x, y, 48, 48, IC_CONTROL_POINT_DUPLICATE);
      break;

    case  177 :
      display->drawXbm(x, y, 48, 48, IC_COPYRIGHT);
      break;

    case  178 :
      display->drawXbm(x, y, 48, 48, IC_CREATE);
      break;

    case  179 :
      display->drawXbm(x, y, 48, 48, IC_CREATE_NEW_FOLDER);
      break;

    case  180 :
      display->drawXbm(x, y, 48, 48, IC_CREDIT_CARD);
      break;

    case  181 :
      display->drawXbm(x, y, 48, 48, IC_CROP_16_9);
      break;

    case  182 :
      display->drawXbm(x, y, 48, 48, IC_CROP_3_2);
      break;

    case  183 :
      display->drawXbm(x, y, 48, 48, IC_CROP_5_4);
      break;

    case  184 :
      display->drawXbm(x, y, 48, 48, IC_CROP_7_5);
      break;

    case  185 :
      display->drawXbm(x, y, 48, 48, IC_CROP);
      break;

    case  186 :
      display->drawXbm(x, y, 48, 48, IC_CROP_DIN);
      break;

    case  187 :
      display->drawXbm(x, y, 48, 48, IC_CROP_FREE);
      break;

    case  188 :
      display->drawXbm(x, y, 48, 48, IC_CROP_LANDSCAPE);
      break;

    case  189 :
      display->drawXbm(x, y, 48, 48, IC_CROP_ORIGINAL);
      break;

    case  190 :
      display->drawXbm(x, y, 48, 48, IC_CROP_PORTRAIT);
      break;

    case  191 :
      display->drawXbm(x, y, 48, 48, IC_CROP_ROTATE);
      break;

    case  192 :
      display->drawXbm(x, y, 48, 48, IC_CROP_SQUARE);
      break;

    case  193 :
      display->drawXbm(x, y, 48, 48, IC_DASHBOARD);
      break;

    case  194 :
      display->drawXbm(x, y, 48, 48, IC_DATE_RANGE);
      break;

    case  195 :
      display->drawXbm(x, y, 48, 48, IC_DEHAZE);
      break;

    case  196 :
      display->drawXbm(x, y, 48, 48, IC_DELETE);
      break;

    case  197 :
      display->drawXbm(x, y, 48, 48, IC_DELETE_FOREVER);
      break;

    case  198 :
      display->drawXbm(x, y, 48, 48, IC_DELETE_SWEEP);
      break;

    case  199 :
      display->drawXbm(x, y, 48, 48, IC_DESCRIPTION);
      break;

    case  200 :
      display->drawXbm(x, y, 48, 48, IC_DESKTOP_MAC);
      break;

    case  201 :
      display->drawXbm(x, y, 48, 48, IC_DESKTOP_WINDOWS);
      break;

    case  202 :
      display->drawXbm(x, y, 48, 48, IC_DETAILS);
      break;

    case  203 :
      display->drawXbm(x, y, 48, 48, IC_DEVELOPER_BOARD);
      break;

    case  204 :
      display->drawXbm(x, y, 48, 48, IC_DEVICES_OTHER);
      break;

    case  205 :
      display->drawXbm(x, y, 48, 48, IC_DEVICE_HUB);
      break;

    case  206 :
      display->drawXbm(x, y, 48, 48, IC_DIALER_SIP);
      break;

    case  207 :
      display->drawXbm(x, y, 48, 48, IC_DIALPAD);
      break;

    case  208 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_BIKE);
      break;

    case  209 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS);
      break;

    case  210 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_BOAT);
      break;

    case  211 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_BUS);
      break;

    case  212 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_CAR);
      break;

    case  213 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_RAILWAY);
      break;

    case  214 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_RUN);
      break;

    case  215 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_SUBWAY);
      break;

    case  216 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_TRANSIT);
      break;

    case  217 :
      display->drawXbm(x, y, 48, 48, IC_DIRECTIONS_WALK);
      break;

    case  218 :
      display->drawXbm(x, y, 48, 48, IC_DISC_FULL);
      break;

    case  219 :
      display->drawXbm(x, y, 48, 48, IC_DNS);
      break;

    case  220 :
      display->drawXbm(x, y, 48, 48, IC_DOCK);
      break;

    case  221 :
      display->drawXbm(x, y, 48, 48, IC_DOMAIN);
      break;

    case  222 :
      display->drawXbm(x, y, 48, 48, IC_DONE_ALL);
      break;

    case  223 :
      display->drawXbm(x, y, 48, 48, IC_DONE);
      break;

    case  224 :
      display->drawXbm(x, y, 48, 48, IC_DONUT_LARGE);
      break;

    case  225 :
      display->drawXbm(x, y, 48, 48, IC_DONUT_SMALL);
      break;

    case  226 :
      display->drawXbm(x, y, 48, 48, IC_DO_NOT_DISTURB_ALT);
      break;

    case  227 :
      display->drawXbm(x, y, 48, 48, IC_DO_NOT_DISTURB);
      break;

    case  228 :
      display->drawXbm(x, y, 48, 48, IC_DO_NOT_DISTURB_OFF);
      break;

    case  229 :
      display->drawXbm(x, y, 48, 48, IC_DO_NOT_DISTURB_ON);
      break;

    case  230 :
      display->drawXbm(x, y, 48, 48, IC_DRAFTS);
      break;

    case  231 :
      display->drawXbm(x, y, 48, 48, IC_DRAG_HANDLE);
      break;

    case  232 :
      display->drawXbm(x, y, 48, 48, IC_DRIVE_ETA);
      break;

    case  233 :
      display->drawXbm(x, y, 48, 48, IC_EDIT);
      break;

    case  234 :
      display->drawXbm(x, y, 48, 48, IC_EDIT_LOCATION);
      break;

    case  235 :
      display->drawXbm(x, y, 48, 48, IC_EJECT);
      break;

    case  236 :
      display->drawXbm(x, y, 48, 48, IC_EMAIL);
      break;

    case  237 :
      display->drawXbm(x, y, 48, 48, IC_ENHANCED_ENCRYPTION);
      break;

    case  238 :
      display->drawXbm(x, y, 48, 48, IC_EQUALIZER);
      break;

    case  239 :
      display->drawXbm(x, y, 48, 48, IC_ERROR);
      break;

    case  240 :
      display->drawXbm(x, y, 48, 48, IC_ERROR_OUTLINE);
      break;

    case  241 :
      display->drawXbm(x, y, 48, 48, IC_EURO_SYMBOL);
      break;

    case  242 :
      display->drawXbm(x, y, 48, 48, IC_EVENT_AVAILABLE);
      break;

    case  243 :
      display->drawXbm(x, y, 48, 48, IC_EVENT);
      break;

    case  244 :
      display->drawXbm(x, y, 48, 48, IC_EVENT_BUSY);
      break;

    case  245 :
      display->drawXbm(x, y, 48, 48, IC_EVENT_NOTE);
      break;

    case  246 :
      display->drawXbm(x, y, 48, 48, IC_EVENT_SEAT);
      break;

    case  247 :
      display->drawXbm(x, y, 48, 48, IC_EV_STATION);
      break;

    case  248 :
      display->drawXbm(x, y, 48, 48, IC_EXIT_TO_APP);
      break;

    case  249 :
      display->drawXbm(x, y, 48, 48, IC_EXPAND_LESS);
      break;

    case  250 :
      display->drawXbm(x, y, 48, 48, IC_EXPAND_MORE);
      break;

    case  251 :
      display->drawXbm(x, y, 48, 48, IC_EXPLICIT);
      break;

    case  252 :
      display->drawXbm(x, y, 48, 48, IC_EXPLORE);
      break;

    case  253 :
      display->drawXbm(x, y, 48, 48, IC_EXPOSURE);
      break;

    case  254 :
      display->drawXbm(x, y, 48, 48, IC_EXPOSURE_NEG_1);
      break;

    case  255 :
      display->drawXbm(x, y, 48, 48, IC_EXPOSURE_NEG_2);
      break;

    case  256 :
      display->drawXbm(x, y, 48, 48, IC_EXPOSURE_PLUS_1);
      break;

    case  257 :
      display->drawXbm(x, y, 48, 48, IC_EXPOSURE_PLUS_2);
      break;

    case  258 :
      display->drawXbm(x, y, 48, 48, IC_EXPOSURE_ZERO);
      break;

    case  259 :
      display->drawXbm(x, y, 48, 48, IC_EXTENSION);
      break;

    case  260 :
      display->drawXbm(x, y, 48, 48, IC_FACE);
      break;

    case  261 :
      display->drawXbm(x, y, 48, 48, IC_FAST_FORWARD);
      break;

    case  262 :
      display->drawXbm(x, y, 48, 48, IC_FAST_REWIND);
      break;

    case  263 :
      display->drawXbm(x, y, 48, 48, IC_FAVORITE);
      break;

    case  264 :
      display->drawXbm(x, y, 48, 48, IC_FAVORITE_BORDER);
      break;

    case  265 :
      display->drawXbm(x, y, 48, 48, IC_FEATURED_PLAY_LIST);
      break;

    case  266 :
      display->drawXbm(x, y, 48, 48, IC_FEATURED_VIDEO);
      break;

    case  267 :
      display->drawXbm(x, y, 48, 48, IC_FEEDBACK);
      break;

    case  268 :
      display->drawXbm(x, y, 48, 48, IC_FIBER_DVR);
      break;

    case  269 :
      display->drawXbm(x, y, 48, 48, IC_FIBER_MANUAL_RECORD);
      break;

    case  270 :
      display->drawXbm(x, y, 48, 48, IC_FIBER_NEW);
      break;

    case  271 :
      display->drawXbm(x, y, 48, 48, IC_FIBER_PIN);
      break;

    case  272 :
      display->drawXbm(x, y, 48, 48, IC_FIBER_SMART_RECORD);
      break;

    case  273 :
      display->drawXbm(x, y, 48, 48, IC_FILE_DOWNLOAD);
      break;

    case  274 :
      display->drawXbm(x, y, 48, 48, IC_FILE_UPLOAD);
      break;

    case  275 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_1);
      break;

    case  276 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_2);
      break;

    case  277 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_3);
      break;

    case  278 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_4);
      break;

    case  279 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_5);
      break;

    case  280 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_6);
      break;

    case  281 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_7);
      break;

    case  282 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_8);
      break;

    case  283 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_9);
      break;

    case  284 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_9_PLUS);
      break;

    case  285 :
      display->drawXbm(x, y, 48, 48, IC_FILTER);
      break;

    case  286 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_B_AND_W);
      break;

    case  287 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_CENTER_FOCUS);
      break;

    case  288 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_DRAMA);
      break;

    case  289 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_FRAMES);
      break;

    case  290 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_HDR);
      break;

    case  291 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_LIST);
      break;

    case  292 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_NONE);
      break;

    case  293 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_TILT_SHIFT);
      break;

    case  294 :
      display->drawXbm(x, y, 48, 48, IC_FILTER_VINTAGE);
      break;

    case  295 :
      display->drawXbm(x, y, 48, 48, IC_FIND_IN_PAGE);
      break;

    case  296 :
      display->drawXbm(x, y, 48, 48, IC_FIND_REPLACE);
      break;

    case  297 :
      display->drawXbm(x, y, 48, 48, IC_FINGERPRINT);
      break;

    case  298 :
      display->drawXbm(x, y, 48, 48, IC_FIRST_PAGE);
      break;

    case  299 :
      display->drawXbm(x, y, 48, 48, IC_FITNESS_CENTER);
      break;

    case  300 :
      display->drawXbm(x, y, 48, 48, IC_FLAG);
      break;

    case  301 :
      display->drawXbm(x, y, 48, 48, IC_FLARE);
      break;

    case  302 :
      display->drawXbm(x, y, 48, 48, IC_FLASH_AUTO);
      break;

    case  303 :
      display->drawXbm(x, y, 48, 48, IC_FLASH_OFF);
      break;

    case  304 :
      display->drawXbm(x, y, 48, 48, IC_FLASH_ON);
      break;

    case  305 :
      display->drawXbm(x, y, 48, 48, IC_FLIGHT);
      break;

    case  306 :
      display->drawXbm(x, y, 48, 48, IC_FLIGHT_LAND);
      break;

    case  307 :
      display->drawXbm(x, y, 48, 48, IC_FLIGHT_TAKEOFF);
      break;

    case  308 :
      display->drawXbm(x, y, 48, 48, IC_FLIP);
      break;

    case  309 :
      display->drawXbm(x, y, 48, 48, IC_FLIP_TO_BACK);
      break;

    case  310 :
      display->drawXbm(x, y, 48, 48, IC_FLIP_TO_FRONT);
      break;

    case  311 :
      display->drawXbm(x, y, 48, 48, IC_FOLDER);
      break;

    case  312 :
      display->drawXbm(x, y, 48, 48, IC_FOLDER_OPEN);
      break;

    case  313 :
      display->drawXbm(x, y, 48, 48, IC_FOLDER_SHARED);
      break;

    case  314 :
      display->drawXbm(x, y, 48, 48, IC_FOLDER_SPECIAL);
      break;

    case  315 :
      display->drawXbm(x, y, 48, 48, IC_FONT_DOWNLOAD);
      break;

    case  316 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_ALIGN_CENTER);
      break;

    case  317 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_ALIGN_JUSTIFY);
      break;

    case  318 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_ALIGN_LEFT);
      break;

    case  319 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_ALIGN_RIGHT);
      break;

    case  320 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_BOLD);
      break;

    case  321 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_CLEAR);
      break;

    case  322 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_COLOR_FILL);
      break;

    case  323 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_COLOR_RESET);
      break;

    case  324 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_COLOR_TEXT);
      break;

    case  325 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_INDENT_DECREASE);
      break;

    case  326 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_INDENT_INCREASE);
      break;

    case  327 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_ITALIC);
      break;

    case  328 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_LINE_SPACING);
      break;

    case  329 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_LIST_BULLETED);
      break;

    case  330 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_LIST_NUMBERED);
      break;

    case  331 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_PAINT);
      break;

    case  332 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_QUOTE);
      break;

    case  333 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_SHAPES);
      break;

    case  334 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_SIZE);
      break;

    case  335 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_STRIKETHROUGH);
      break;

    case  336 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_TEXTDIRECTION_L_TO_R);
      break;

    case  337 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_TEXTDIRECTION_R_TO_L);
      break;

    case  338 :
      display->drawXbm(x, y, 48, 48, IC_FORMAT_UNDERLINED);
      break;

    case  339 :
      display->drawXbm(x, y, 48, 48, IC_FORUM);
      break;

    case  340 :
      display->drawXbm(x, y, 48, 48, IC_FORWARD_10);
      break;

    case  341 :
      display->drawXbm(x, y, 48, 48, IC_FORWARD_30);
      break;

    case  342 :
      display->drawXbm(x, y, 48, 48, IC_FORWARD_5);
      break;

    case  343 :
      display->drawXbm(x, y, 48, 48, IC_FORWARD);
      break;

    case  344 :
      display->drawXbm(x, y, 48, 48, IC_FREE_BREAKFAST);
      break;

    case  345 :
      display->drawXbm(x, y, 48, 48, IC_FULLSCREEN);
      break;

    case  346 :
      display->drawXbm(x, y, 48, 48, IC_FULLSCREEN_EXIT);
      break;

    case  347 :
      display->drawXbm(x, y, 48, 48, IC_FUNCTIONS);
      break;

    case  348 :
      display->drawXbm(x, y, 48, 48, IC_GAMEPAD);
      break;

    case  349 :
      display->drawXbm(x, y, 48, 48, IC_GAMES);
      break;

    case  350 :
      display->drawXbm(x, y, 48, 48, IC_GAVEL);
      break;

    case  351 :
      display->drawXbm(x, y, 48, 48, IC_GESTURE);
      break;

    case  352 :
      display->drawXbm(x, y, 48, 48, IC_GET_APP);
      break;

    case  353 :
      display->drawXbm(x, y, 48, 48, IC_GIF);
      break;

    case  354 :
      display->drawXbm(x, y, 48, 48, IC_GOLF_COURSE);
      break;

    case  355 :
      display->drawXbm(x, y, 48, 48, IC_GRADE);
      break;

    case  356 :
      display->drawXbm(x, y, 48, 48, IC_GRADIENT);
      break;

    case  357 :
      display->drawXbm(x, y, 48, 48, IC_GRAIN);
      break;

    case  358 :
      display->drawXbm(x, y, 48, 48, IC_GRID_OFF);
      break;

    case  359 :
      display->drawXbm(x, y, 48, 48, IC_GRID_ON);
      break;

    case  360 :
      display->drawXbm(x, y, 48, 48, IC_GROUP_ADD);
      break;

    case  361 :
      display->drawXbm(x, y, 48, 48, IC_GROUP);
      break;

    case  362 :
      display->drawXbm(x, y, 48, 48, IC_GROUP_WORK);
      break;

    case  363 :
      display->drawXbm(x, y, 48, 48, IC_G_TRANSLATE);
      break;

    case  364 :
      display->drawXbm(x, y, 48, 48, IC_HDR_OFF);
      break;

    case  365 :
      display->drawXbm(x, y, 48, 48, IC_HDR_ON);
      break;

    case  366 :
      display->drawXbm(x, y, 48, 48, IC_HDR_STRONG);
      break;

    case  367 :
      display->drawXbm(x, y, 48, 48, IC_HDR_WEAK);
      break;

    case  368 :
      display->drawXbm(x, y, 48, 48, IC_HD);
      break;

    case  369 :
      display->drawXbm(x, y, 48, 48, IC_HEADSET);
      break;

    case  370 :
      display->drawXbm(x, y, 48, 48, IC_HEADSET_MIC);
      break;

    case  371 :
      display->drawXbm(x, y, 48, 48, IC_HEALING);
      break;

    case  372 :
      display->drawXbm(x, y, 48, 48, IC_HEARING);
      break;

    case  373 :
      display->drawXbm(x, y, 48, 48, IC_HELP);
      break;

    case  374 :
      display->drawXbm(x, y, 48, 48, IC_HELP_OUTLINE);
      break;

    case  375 :
      display->drawXbm(x, y, 48, 48, IC_HIGHLIGHT);
      break;

    case  376 :
      display->drawXbm(x, y, 48, 48, IC_HIGHLIGHT_OFF);
      break;

    case  377 :
      display->drawXbm(x, y, 48, 48, IC_HIGH_QUALITY);
      break;

    case  378 :
      display->drawXbm(x, y, 48, 48, IC_HISTORY);
      break;

    case  379 :
      display->drawXbm(x, y, 48, 48, IC_HOME);
      break;

    case  380 :
      display->drawXbm(x, y, 48, 48, IC_HOTEL);
      break;

    case  381 :
      display->drawXbm(x, y, 48, 48, IC_HOT_TUB);
      break;

    case  382 :
      display->drawXbm(x, y, 48, 48, IC_HOURGLASS_EMPTY);
      break;

    case  383 :
      display->drawXbm(x, y, 48, 48, IC_HOURGLASS_FULL);
      break;

    case  384 :
      display->drawXbm(x, y, 48, 48, IC_HTTPS);
      break;

    case  385 :
      display->drawXbm(x, y, 48, 48, IC_HTTP);
      break;

    case  386 :
      display->drawXbm(x, y, 48, 48, IC_IMAGE_ASPECT_RATIO);
      break;

    case  387 :
      display->drawXbm(x, y, 48, 48, IC_IMAGE);
      break;

    case  388 :
      display->drawXbm(x, y, 48, 48, IC_IMPORTANT_DEVICES);
      break;

    case  389 :
      display->drawXbm(x, y, 48, 48, IC_IMPORT_CONTACTS);
      break;

    case  390 :
      display->drawXbm(x, y, 48, 48, IC_IMPORT_EXPORT);
      break;

    case  391 :
      display->drawXbm(x, y, 48, 48, IC_INBOX);
      break;

    case  392 :
      display->drawXbm(x, y, 48, 48, IC_INDETERMINATE_CHECK_BOX);
      break;

    case  393 :
      display->drawXbm(x, y, 48, 48, IC_INFO);
      break;

    case  394 :
      display->drawXbm(x, y, 48, 48, IC_INFO_OUTLINE);
      break;

    case  395 :
      display->drawXbm(x, y, 48, 48, IC_INPUT);
      break;

    case  396 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_CHART);
      break;

    case  397 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_COMMENT);
      break;

    case  398 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_DRIVE_FILE);
      break;

    case  399 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_EMOTICON);
      break;

    case  400 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_INVITATION);
      break;

    case  401 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_LINK);
      break;

    case  402 :
      display->drawXbm(x, y, 48, 48, IC_INSERT_PHOTO);
      break;

    case  403 :
      display->drawXbm(x, y, 48, 48, IC_INVERT_COLORS);
      break;

    case  404 :
      display->drawXbm(x, y, 48, 48, IC_INVERT_COLORS_OFF);
      break;

    case  405 :
      display->drawXbm(x, y, 48, 48, IC_ISO);
      break;

    case  406 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_ARROW_DOWN);
      break;

    case  407 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_ARROW_LEFT);
      break;

    case  408 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_ARROW_RIGHT);
      break;

    case  409 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_ARROW_UP);
      break;

    case  410 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_BACKSPACE);
      break;

    case  411 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD);
      break;

    case  412 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_CAPSLOCK);
      break;

    case  413 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_HIDE);
      break;

    case  414 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_RETURN);
      break;

    case  415 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_TAB);
      break;

    case  416 :
      display->drawXbm(x, y, 48, 48, IC_KEYBOARD_VOICE);
      break;

    case  417 :
      display->drawXbm(x, y, 48, 48, IC_KITCHEN);
      break;

    case  418 :
      display->drawXbm(x, y, 48, 48, IC_LABEL);
      break;

    case  419 :
      display->drawXbm(x, y, 48, 48, IC_LABEL_OUTLINE);
      break;

    case  420 :
      display->drawXbm(x, y, 48, 48, IC_LANDSCAPE);
      break;

    case  421 :
      display->drawXbm(x, y, 48, 48, IC_LANGUAGE);
      break;

    case  422 :
      display->drawXbm(x, y, 48, 48, IC_LAPTOP);
      break;

    case  423 :
      display->drawXbm(x, y, 48, 48, IC_LAPTOP_CHROMEBOOK);
      break;

    case  424 :
      display->drawXbm(x, y, 48, 48, IC_LAPTOP_MAC);
      break;

    case  425 :
      display->drawXbm(x, y, 48, 48, IC_LAPTOP_WINDOWS);
      break;

    case  426 :
      display->drawXbm(x, y, 48, 48, IC_LAST_PAGE);
      break;

    case  427 :
      display->drawXbm(x, y, 48, 48, IC_LAUNCH);
      break;

    case  428 :
      display->drawXbm(x, y, 48, 48, IC_LAYERS);
      break;

    case  429 :
      display->drawXbm(x, y, 48, 48, IC_LAYERS_CLEAR);
      break;

    case  430 :
      display->drawXbm(x, y, 48, 48, IC_LEAK_ADD);
      break;

    case  431 :
      display->drawXbm(x, y, 48, 48, IC_LEAK_REMOVE);
      break;

    case  432 :
      display->drawXbm(x, y, 48, 48, IC_LENS);
      break;

    case  433 :
      display->drawXbm(x, y, 48, 48, IC_LIBRARY_ADD);
      break;

    case  434 :
      display->drawXbm(x, y, 48, 48, IC_LIBRARY_BOOKS);
      break;

    case  435 :
      display->drawXbm(x, y, 48, 48, IC_LIBRARY_MUSIC);
      break;

    case  436 :
      display->drawXbm(x, y, 48, 48, IC_LIGHTBULB_OUTLINE);
      break;

    case  437 :
      display->drawXbm(x, y, 48, 48, IC_LINEAR_SCALE);
      break;

    case  438 :
      display->drawXbm(x, y, 48, 48, IC_LINE_STYLE);
      break;

    case  439 :
      display->drawXbm(x, y, 48, 48, IC_LINE_WEIGHT);
      break;

    case  440 :
      display->drawXbm(x, y, 48, 48, IC_LINKED_CAMERA);
      break;

    case  441 :
      display->drawXbm(x, y, 48, 48, IC_LINK);
      break;

    case  442 :
      display->drawXbm(x, y, 48, 48, IC_LIST);
      break;

    case  443 :
      display->drawXbm(x, y, 48, 48, IC_LIVE_HELP);
      break;

    case  444 :
      display->drawXbm(x, y, 48, 48, IC_LIVE_TV);
      break;

    case  445 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_ACTIVITY);
      break;

    case  446 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_AIRPORT);
      break;

    case  447 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_ATM);
      break;

    case  448 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_BAR);
      break;

    case  449 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_CAFE);
      break;

    case  450 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_CAR_WASH);
      break;

    case  451 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_CONVENIENCE_STORE);
      break;

    case  452 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_DINING);
      break;

    case  453 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_DRINK);
      break;

    case  454 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_FLORIST);
      break;

    case  455 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_GAS_STATION);
      break;

    case  456 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_GROCERY_STORE);
      break;

    case  457 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_HOSPITAL);
      break;

    case  458 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_HOTEL);
      break;

    case  459 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_LAUNDRY_SERVICE);
      break;

    case  460 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_LIBRARY);
      break;

    case  461 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_MALL);
      break;

    case  462 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_MOVIES);
      break;

    case  463 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_OFFER);
      break;

    case  464 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_PARKING);
      break;

    case  465 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_PHARMACY);
      break;

    case  466 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_PHONE);
      break;

    case  467 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_PIZZA);
      break;

    case  468 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_PLAY);
      break;

    case  469 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_POST_OFFICE);
      break;

    case  470 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_PRINTSHOP);
      break;

    case  471 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_SEE);
      break;

    case  472 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_SHIPPING);
      break;

    case  473 :
      display->drawXbm(x, y, 48, 48, IC_LOCAL_TAXI);
      break;

    case  474 :
      display->drawXbm(x, y, 48, 48, IC_LOCATION_CITY);
      break;

    case  475 :
      display->drawXbm(x, y, 48, 48, IC_LOCATION_OFF);
      break;

    case  476 :
      display->drawXbm(x, y, 48, 48, IC_LOCATION_ON);
      break;

    case  477 :
      display->drawXbm(x, y, 48, 48, IC_LOCK);
      break;

    case  478 :
      display->drawXbm(x, y, 48, 48, IC_LOCK_OPEN);
      break;

    case  479 :
      display->drawXbm(x, y, 48, 48, IC_LOCK_OUTLINE);
      break;

    case  480 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS_3);
      break;

    case  481 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS_4);
      break;

    case  482 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS_5);
      break;

    case  483 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS_6);
      break;

    case  484 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS);
      break;

    case  485 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS_ONE);
      break;

    case  486 :
      display->drawXbm(x, y, 48, 48, IC_LOOKS_TWO);
      break;

    case  487 :
      display->drawXbm(x, y, 48, 48, IC_LOOP);
      break;

    case  488 :
      display->drawXbm(x, y, 48, 48, IC_LOUPE);
      break;

    case  489 :
      display->drawXbm(x, y, 48, 48, IC_LOW_PRIORITY);
      break;

    case  490 :
      display->drawXbm(x, y, 48, 48, IC_LOYALTY);
      break;

    case  491 :
      display->drawXbm(x, y, 48, 48, IC_MAIL);
      break;

    case  492 :
      display->drawXbm(x, y, 48, 48, IC_MAIL_OUTLINE);
      break;

    case  493 :
      display->drawXbm(x, y, 48, 48, IC_MAP);
      break;

    case  494 :
      display->drawXbm(x, y, 48, 48, IC_MARKUNREAD);
      break;

    case  495 :
      display->drawXbm(x, y, 48, 48, IC_MARKUNREAD_MAILBOX);
      break;

    case  496 :
      display->drawXbm(x, y, 48, 48, IC_MEMORY);
      break;

    case  497 :
      display->drawXbm(x, y, 48, 48, IC_MENU);
      break;

    case  498 :
      display->drawXbm(x, y, 48, 48, IC_MERGE_TYPE);
      break;

    case  499 :
      display->drawXbm(x, y, 48, 48, IC_MESSAGE);
      break;

    case  500 :
      display->drawXbm(x, y, 48, 48, IC_MIC);
      break;

    case  501 :
      display->drawXbm(x, y, 48, 48, IC_MIC_NONE);
      break;

    case  502 :
      display->drawXbm(x, y, 48, 48, IC_MIC_OFF);
      break;

    case  503 :
      display->drawXbm(x, y, 48, 48, IC_MMS);
      break;

    case  504 :
      display->drawXbm(x, y, 48, 48, IC_MODE_COMMENT);
      break;

    case  505 :
      display->drawXbm(x, y, 48, 48, IC_MODE_EDIT);
      break;

    case  506 :
      display->drawXbm(x, y, 48, 48, IC_MONETIZATION_ON);
      break;

    case  507 :
      display->drawXbm(x, y, 48, 48, IC_MONEY_OFF);
      break;

    case  508 :
      display->drawXbm(x, y, 48, 48, IC_MONOCHROME_PHOTOS);
      break;

    case  509 :
      display->drawXbm(x, y, 48, 48, IC_MOOD_BAD);
      break;

    case  510 :
      display->drawXbm(x, y, 48, 48, IC_MOOD);
      break;

    case  511 :
      display->drawXbm(x, y, 48, 48, IC_MORE);
      break;

    case  512 :
      display->drawXbm(x, y, 48, 48, IC_MORE_HORIZ);
      break;

    case  513 :
      display->drawXbm(x, y, 48, 48, IC_MORE_VERT);
      break;

    case  514 :
      display->drawXbm(x, y, 48, 48, IC_MOTORCYCLE);
      break;

    case  515 :
      display->drawXbm(x, y, 48, 48, IC_MOUSE);
      break;

    case  516 :
      display->drawXbm(x, y, 48, 48, IC_MOVE_TO_INBOX);
      break;

    case  517 :
      display->drawXbm(x, y, 48, 48, IC_MOVIE);
      break;

    case  518 :
      display->drawXbm(x, y, 48, 48, IC_MOVIE_CREATION);
      break;

    case  519 :
      display->drawXbm(x, y, 48, 48, IC_MOVIE_FILTER);
      break;

    case  520 :
      display->drawXbm(x, y, 48, 48, IC_MULTILINE_CHART);
      break;

    case  521 :
      display->drawXbm(x, y, 48, 48, IC_MUSIC_NOTE);
      break;

    case  522 :
      display->drawXbm(x, y, 48, 48, IC_MUSIC_VIDEO);
      break;

    case  523 :
      display->drawXbm(x, y, 48, 48, IC_MY_LOCATION);
      break;

    case  524 :
      display->drawXbm(x, y, 48, 48, IC_NATURE);
      break;

    case  525 :
      display->drawXbm(x, y, 48, 48, IC_NATURE_PEOPLE);
      break;

    case  526 :
      display->drawXbm(x, y, 48, 48, IC_NAVIGATE_BEFORE);
      break;

    case  527 :
      display->drawXbm(x, y, 48, 48, IC_NAVIGATE_NEXT);
      break;

    case  528 :
      display->drawXbm(x, y, 48, 48, IC_NAVIGATION);
      break;

    case  529 :
      display->drawXbm(x, y, 48, 48, IC_NEAR_ME);
      break;

    case  530 :
      display->drawXbm(x, y, 48, 48, IC_NETWORK_CHECK);
      break;

    case  531 :
      display->drawXbm(x, y, 48, 48, IC_NETWORK_LOCKED);
      break;

    case  532 :
      display->drawXbm(x, y, 48, 48, IC_NEW_RELEASES);
      break;

    case  533 :
      display->drawXbm(x, y, 48, 48, IC_NEXT_WEEK);
      break;

    case  534 :
      display->drawXbm(x, y, 48, 48, IC_NOTE_ADD);
      break;

    case  535 :
      display->drawXbm(x, y, 48, 48, IC_NOTE);
      break;

    case  536 :
      display->drawXbm(x, y, 48, 48, IC_NOTIFICATIONS_ACTIVE);
      break;

    case  537 :
      display->drawXbm(x, y, 48, 48, IC_NOTIFICATIONS);
      break;

    case  538 :
      display->drawXbm(x, y, 48, 48, IC_NOTIFICATIONS_NONE);
      break;

    case  539 :
      display->drawXbm(x, y, 48, 48, IC_NOTIFICATIONS_OFF);
      break;

    case  540 :
      display->drawXbm(x, y, 48, 48, IC_NOTIFICATIONS_PAUSED);
      break;

    case  541 :
      display->drawXbm(x, y, 48, 48, IC_NOT_INTERESTED);
      break;

    case  542 :
      display->drawXbm(x, y, 48, 48, IC_NO_ENCRYPTION);
      break;

    case  543 :
      display->drawXbm(x, y, 48, 48, IC_NO_SIM);
      break;

    case  544 :
      display->drawXbm(x, y, 48, 48, IC_OFFLINE_PIN);
      break;

    case  545 :
      display->drawXbm(x, y, 48, 48, IC_ONDEMAND_VIDEO);
      break;

    case  546 :
      display->drawXbm(x, y, 48, 48, IC_OPACITY);
      break;

    case  547 :
      display->drawXbm(x, y, 48, 48, IC_OPEN_IN_BROWSER);
      break;

    case  548 :
      display->drawXbm(x, y, 48, 48, IC_OPEN_IN_NEW);
      break;

    case  549 :
      display->drawXbm(x, y, 48, 48, IC_OPEN_WITH);
      break;

    case  550 :
      display->drawXbm(x, y, 48, 48, IC_PAGES);
      break;

    case  551 :
      display->drawXbm(x, y, 48, 48, IC_PAGEVIEW);
      break;

    case  552 :
      display->drawXbm(x, y, 48, 48, IC_PALETTE);
      break;

    case  553 :
      display->drawXbm(x, y, 48, 48, IC_PANORAMA);
      break;

    case  554 :
      display->drawXbm(x, y, 48, 48, IC_PANORAMA_FISH_EYE);
      break;

    case  555 :
      display->drawXbm(x, y, 48, 48, IC_PANORAMA_HORIZONTAL);
      break;

    case  556 :
      display->drawXbm(x, y, 48, 48, IC_PANORAMA_VERTICAL);
      break;

    case  557 :
      display->drawXbm(x, y, 48, 48, IC_PANORAMA_WIDE_ANGLE);
      break;

    case  558 :
      display->drawXbm(x, y, 48, 48, IC_PAN_TOOL);
      break;

    case  559 :
      display->drawXbm(x, y, 48, 48, IC_PARTY_MODE);
      break;

    case  560 :
      display->drawXbm(x, y, 48, 48, IC_PAUSE);
      break;

    case  561 :
      display->drawXbm(x, y, 48, 48, IC_PAUSE_CIRCLE_FILLED);
      break;

    case  562 :
      display->drawXbm(x, y, 48, 48, IC_PAUSE_CIRCLE_OUTLINE);
      break;

    case  563 :
      display->drawXbm(x, y, 48, 48, IC_PAYMENT);
      break;

    case  564 :
      display->drawXbm(x, y, 48, 48, IC_PEOPLE);
      break;

    case  565 :
      display->drawXbm(x, y, 48, 48, IC_PEOPLE_OUTLINE);
      break;

    case  566 :
      display->drawXbm(x, y, 48, 48, IC_PERM_CAMERA_MIC);
      break;

    case  567 :
      display->drawXbm(x, y, 48, 48, IC_PERM_CONTACT_CALENDAR);
      break;

    case  568 :
      display->drawXbm(x, y, 48, 48, IC_PERM_DATA_SETTING);
      break;

    case  569 :
      display->drawXbm(x, y, 48, 48, IC_PERM_DEVICE_INFORMATION);
      break;

    case  570 :
      display->drawXbm(x, y, 48, 48, IC_PERM_IDENTITY);
      break;

    case  571 :
      display->drawXbm(x, y, 48, 48, IC_PERM_MEDIA);
      break;

    case  572 :
      display->drawXbm(x, y, 48, 48, IC_PERM_PHONE_MSG);
      break;

    case  573 :
      display->drawXbm(x, y, 48, 48, IC_PERM_SCAN_WIFI);
      break;

    case  574 :
      display->drawXbm(x, y, 48, 48, IC_PERSONAL_VIDEO);
      break;

    case  575 :
      display->drawXbm(x, y, 48, 48, IC_PERSON_ADD);
      break;

    case  576 :
      display->drawXbm(x, y, 48, 48, IC_PERSON);
      break;

    case  577 :
      display->drawXbm(x, y, 48, 48, IC_PERSON_OUTLINE);
      break;

    case  578 :
      display->drawXbm(x, y, 48, 48, IC_PERSON_PIN);
      break;

    case  579 :
      display->drawXbm(x, y, 48, 48, IC_PERSON_PIN_CIRCLE);
      break;

    case  580 :
      display->drawXbm(x, y, 48, 48, IC_PETS);
      break;

    case  581 :
      display->drawXbm(x, y, 48, 48, IC_PHONELINK);
      break;

    case  582 :
      display->drawXbm(x, y, 48, 48, IC_PHONELINK_ERASE);
      break;

    case  583 :
      display->drawXbm(x, y, 48, 48, IC_PHONELINK_LOCK);
      break;

    case  584 :
      display->drawXbm(x, y, 48, 48, IC_PHONELINK_OFF);
      break;

    case  585 :
      display->drawXbm(x, y, 48, 48, IC_PHONELINK_RING);
      break;

    case  586 :
      display->drawXbm(x, y, 48, 48, IC_PHONELINK_SETUP);
      break;

    case  587 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_ANDROID);
      break;

    case  588 :
      display->drawXbm(x, y, 48, 48, IC_PHONE);
      break;

    case  589 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_BLUETOOTH_SPEAKER);
      break;

    case  590 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_FORWARDED);
      break;

    case  591 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_IN_TALK);
      break;

    case  592 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_IPHONE);
      break;

    case  593 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_LOCKED);
      break;

    case  594 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_MISSED);
      break;

    case  595 :
      display->drawXbm(x, y, 48, 48, IC_PHONE_PAUSED);
      break;

    case  596 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_ALBUM);
      break;

    case  597 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO);
      break;

    case  598 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_CAMERA);
      break;

    case  599 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_FILTER);
      break;

    case  600 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_LIBRARY);
      break;

    case  601 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_SIZE_SELECT_ACTUAL);
      break;

    case  602 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_SIZE_SELECT_LARGE);
      break;

    case  603 :
      display->drawXbm(x, y, 48, 48, IC_PHOTO_SIZE_SELECT_SMALL);
      break;

    case  604 :
      display->drawXbm(x, y, 48, 48, IC_PICTURE_AS_PDF);
      break;

    case  605 :
      display->drawXbm(x, y, 48, 48, IC_PICTURE_IN_PICTURE_ALT);
      break;

    case  606 :
      display->drawXbm(x, y, 48, 48, IC_PICTURE_IN_PICTURE);
      break;

    case  607 :
      display->drawXbm(x, y, 48, 48, IC_PIE_CHART);
      break;

    case  608 :
      display->drawXbm(x, y, 48, 48, IC_PIE_CHART_OUTLINED);
      break;

    case  609 :
      display->drawXbm(x, y, 48, 48, IC_PIN_DROP);
      break;

    case  610 :
      display->drawXbm(x, y, 48, 48, IC_PLACE);
      break;

    case  611 :
      display->drawXbm(x, y, 48, 48, IC_PLAYLIST_ADD);
      break;

    case  612 :
      display->drawXbm(x, y, 48, 48, IC_PLAYLIST_ADD_CHECK);
      break;

    case  613 :
      display->drawXbm(x, y, 48, 48, IC_PLAYLIST_PLAY);
      break;

    case  614 :
      display->drawXbm(x, y, 48, 48, IC_PLAY_ARROW);
      break;

    case  615 :
      display->drawXbm(x, y, 48, 48, IC_PLAY_CIRCLE_FILLED);
      break;

    case  616 :
      display->drawXbm(x, y, 48, 48, IC_PLAY_CIRCLE_FILLED_WHITE);
      break;

    case  617 :
      display->drawXbm(x, y, 48, 48, IC_PLAY_CIRCLE_OUTLINE);
      break;

    case  618 :
      display->drawXbm(x, y, 48, 48, IC_PLAY_FOR_WORK);
      break;

    case  619 :
      display->drawXbm(x, y, 48, 48, IC_PLUS_ONE);
      break;

    case  620 :
      display->drawXbm(x, y, 48, 48, IC_POLL);
      break;

    case  621 :
      display->drawXbm(x, y, 48, 48, IC_POLYMER);
      break;

    case  622 :
      display->drawXbm(x, y, 48, 48, IC_POOL);
      break;

    case  623 :
      display->drawXbm(x, y, 48, 48, IC_PORTABLE_WIFI_OFF);
      break;

    case  624 :
      display->drawXbm(x, y, 48, 48, IC_PORTRAIT);
      break;

    case  625 :
      display->drawXbm(x, y, 48, 48, IC_POWER);
      break;

    case  626 :
      display->drawXbm(x, y, 48, 48, IC_POWER_INPUT);
      break;

    case  627 :
      display->drawXbm(x, y, 48, 48, IC_POWER_SETTINGS_NEW);
      break;

    case  628 :
      display->drawXbm(x, y, 48, 48, IC_PREGNANT_WOMAN);
      break;

    case  629 :
      display->drawXbm(x, y, 48, 48, IC_PRESENT_TO_ALL);
      break;

    case  630 :
      display->drawXbm(x, y, 48, 48, IC_PRINT);
      break;

    case  631 :
      display->drawXbm(x, y, 48, 48, IC_PRIORITY_HIGH);
      break;

    case  632 :
      display->drawXbm(x, y, 48, 48, IC_PUBLIC);
      break;

    case  633 :
      display->drawXbm(x, y, 48, 48, IC_PUBLISH);
      break;

    case  634 :
      display->drawXbm(x, y, 48, 48, IC_QUERY_BUILDER);
      break;

    case  635 :
      display->drawXbm(x, y, 48, 48, IC_QUESTION_ANSWER);
      break;

    case  636 :
      display->drawXbm(x, y, 48, 48, IC_QUEUE);
      break;

    case  637 :
      display->drawXbm(x, y, 48, 48, IC_QUEUE_MUSIC);
      break;

    case  638 :
      display->drawXbm(x, y, 48, 48, IC_QUEUE_PLAY_NEXT);
      break;

    case  639 :
      display->drawXbm(x, y, 48, 48, IC_RADIO);
      break;

    case  640 :
      display->drawXbm(x, y, 48, 48, IC_RADIO_BUTTON_CHECKED);
      break;

    case  641 :
      display->drawXbm(x, y, 48, 48, IC_RADIO_BUTTON_UNCHECKED);
      break;

    case  642 :
      display->drawXbm(x, y, 48, 48, IC_RATE_REVIEW);
      break;

    case  643 :
      display->drawXbm(x, y, 48, 48, IC_RECEIPT);
      break;

    case  644 :
      display->drawXbm(x, y, 48, 48, IC_RECENT_ACTORS);
      break;

    case  645 :
      display->drawXbm(x, y, 48, 48, IC_RECORD_VOICE_OVER);
      break;

    case  646 :
      display->drawXbm(x, y, 48, 48, IC_REDEEM);
      break;

    case  647 :
      display->drawXbm(x, y, 48, 48, IC_REDO);
      break;

    case  648 :
      display->drawXbm(x, y, 48, 48, IC_REFRESH);
      break;

    case  649 :
      display->drawXbm(x, y, 48, 48, IC_REMOVE);
      break;

    case  650 :
      display->drawXbm(x, y, 48, 48, IC_REMOVE_CIRCLE);
      break;

    case  651 :
      display->drawXbm(x, y, 48, 48, IC_REMOVE_CIRCLE_OUTLINE);
      break;

    case  652 :
      display->drawXbm(x, y, 48, 48, IC_REMOVE_FROM_QUEUE);
      break;

    case  653 :
      display->drawXbm(x, y, 48, 48, IC_REMOVE_RED_EYE);
      break;

    case  654 :
      display->drawXbm(x, y, 48, 48, IC_REMOVE_SHOPPING_CART);
      break;

    case  655 :
      display->drawXbm(x, y, 48, 48, IC_REORDER);
      break;

    case  656 :
      display->drawXbm(x, y, 48, 48, IC_REPEAT);
      break;

    case  657 :
      display->drawXbm(x, y, 48, 48, IC_REPEAT_ONE);
      break;

    case  658 :
      display->drawXbm(x, y, 48, 48, IC_REPLAY_10);
      break;

    case  659 :
      display->drawXbm(x, y, 48, 48, IC_REPLAY_30);
      break;

    case  660 :
      display->drawXbm(x, y, 48, 48, IC_REPLAY_5);
      break;

    case  661 :
      display->drawXbm(x, y, 48, 48, IC_REPLAY);
      break;

    case  662 :
      display->drawXbm(x, y, 48, 48, IC_REPLY_ALL);
      break;

    case  663 :
      display->drawXbm(x, y, 48, 48, IC_REPLY);
      break;

    case  664 :
      display->drawXbm(x, y, 48, 48, IC_REPORT);
      break;

    case  665 :
      display->drawXbm(x, y, 48, 48, IC_REPORT_PROBLEM);
      break;

    case  666 :
      display->drawXbm(x, y, 48, 48, IC_RESTAURANT);
      break;

    case  667 :
      display->drawXbm(x, y, 48, 48, IC_RESTAURANT_MENU);
      break;

    case  668 :
      display->drawXbm(x, y, 48, 48, IC_RESTORE);
      break;

    case  669 :
      display->drawXbm(x, y, 48, 48, IC_RESTORE_PAGE);
      break;

    case  670 :
      display->drawXbm(x, y, 48, 48, IC_RING_VOLUME);
      break;

    case  671 :
      display->drawXbm(x, y, 48, 48, IC_ROOM);
      break;

    case  672 :
      display->drawXbm(x, y, 48, 48, IC_ROOM_SERVICE);
      break;

    case  673 :
      display->drawXbm(x, y, 48, 48, IC_ROTATE_90_DEGREES_CCW);
      break;

    case  674 :
      display->drawXbm(x, y, 48, 48, IC_ROTATE_LEFT);
      break;

    case  675 :
      display->drawXbm(x, y, 48, 48, IC_ROTATE_RIGHT);
      break;

    case  676 :
      display->drawXbm(x, y, 48, 48, IC_ROUNDED_CORNER);
      break;

    case  677 :
      display->drawXbm(x, y, 48, 48, IC_ROUTER);
      break;

    case  678 :
      display->drawXbm(x, y, 48, 48, IC_ROWING);
      break;

    case  679 :
      display->drawXbm(x, y, 48, 48, IC_RSS_FEED);
      break;

    case  680 :
      display->drawXbm(x, y, 48, 48, IC_RV_HOOKUP);
      break;

    case  681 :
      display->drawXbm(x, y, 48, 48, IC_SATELLITE);
      break;

    case  682 :
      display->drawXbm(x, y, 48, 48, IC_SAVE);
      break;

    case  683 :
      display->drawXbm(x, y, 48, 48, IC_SCANNER);
      break;

    case  684 :
      display->drawXbm(x, y, 48, 48, IC_SCHEDULE);
      break;

    case  685 :
      display->drawXbm(x, y, 48, 48, IC_SCHOOL);
      break;

    case  686 :
      display->drawXbm(x, y, 48, 48, IC_SCREEN_SHARE);
      break;

    case  687 :
      display->drawXbm(x, y, 48, 48, IC_SD_CARD);
      break;

    case  688 :
      display->drawXbm(x, y, 48, 48, IC_SEARCH);
      break;

    case  689 :
      display->drawXbm(x, y, 48, 48, IC_SECURITY);
      break;

    case  690 :
      display->drawXbm(x, y, 48, 48, IC_SELECT_ALL);
      break;

    case  691 :
      display->drawXbm(x, y, 48, 48, IC_SEND);
      break;

    case  692 :
      display->drawXbm(x, y, 48, 48, IC_SENTIMENT_DISSATISFIED);
      break;

    case  693 :
      display->drawXbm(x, y, 48, 48, IC_SENTIMENT_NEUTRAL);
      break;

    case  694 :
      display->drawXbm(x, y, 48, 48, IC_SENTIMENT_SATISFIED);
      break;

    case  695 :
      display->drawXbm(x, y, 48, 48, IC_SENTIMENT_VERY_DISSATISFIED);
      break;

    case  696 :
      display->drawXbm(x, y, 48, 48, IC_SENTIMENT_VERY_SATISFIED);
      break;

    case  697 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_APPLICATIONS);
      break;

    case  698 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_BACKUP_RESTORE);
      break;

    case  699 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS);
      break;

    case  700 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_BLUETOOTH);
      break;

    case  701 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_BRIGHTNESS);
      break;

    case  702 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_CELL);
      break;

    case  703 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_ETHERNET);
      break;

    case  704 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_INPUT_ANTENNA);
      break;

    case  705 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_INPUT_COMPONENT);
      break;

    case  706 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_INPUT_COMPOSITE);
      break;

    case  707 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_INPUT_HDMI);
      break;

    case  708 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_INPUT_SVIDEO);
      break;

    case  709 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_OVERSCAN);
      break;

    case  710 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_PHONE);
      break;

    case  711 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_POWER);
      break;

    case  712 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_REMOTE);
      break;

    case  713 :
      display->drawXbm(x, y, 48, 48, IC_SETTINGS_VOICE);
      break;

    case  714 :
      display->drawXbm(x, y, 48, 48, IC_SHARE);
      break;

    case  715 :
      display->drawXbm(x, y, 48, 48, IC_SHOPPING_BASKET);
      break;

    case  716 :
      display->drawXbm(x, y, 48, 48, IC_SHOPPING_CART);
      break;

    case  717 :
      display->drawXbm(x, y, 48, 48, IC_SHOP);
      break;

    case  718 :
      display->drawXbm(x, y, 48, 48, IC_SHOP_TWO);
      break;

    case  719 :
      display->drawXbm(x, y, 48, 48, IC_SHORT_TEXT);
      break;

    case  720 :
      display->drawXbm(x, y, 48, 48, IC_SHOW_CHART);
      break;

    case  721 :
      display->drawXbm(x, y, 48, 48, IC_SHUFFLE);
      break;

    case  722 :
      display->drawXbm(x, y, 48, 48, IC_SIM_CARD_ALERT);
      break;

    case  723 :
      display->drawXbm(x, y, 48, 48, IC_SIM_CARD);
      break;

    case  724 :
      display->drawXbm(x, y, 48, 48, IC_SKIP_NEXT);
      break;

    case  725 :
      display->drawXbm(x, y, 48, 48, IC_SKIP_PREVIOUS);
      break;

    case  726 :
      display->drawXbm(x, y, 48, 48, IC_SLIDESHOW);
      break;

    case  727 :
      display->drawXbm(x, y, 48, 48, IC_SLOW_MOTION_VIDEO);
      break;

    case  728 :
      display->drawXbm(x, y, 48, 48, IC_SMARTPHONE);
      break;

    case  729 :
      display->drawXbm(x, y, 48, 48, IC_SMOKE_FREE);
      break;

    case  730 :
      display->drawXbm(x, y, 48, 48, IC_SMOKING_ROOMS);
      break;

    case  731 :
      display->drawXbm(x, y, 48, 48, IC_SMS);
      break;

    case  732 :
      display->drawXbm(x, y, 48, 48, IC_SMS_FAILED);
      break;

    case  733 :
      display->drawXbm(x, y, 48, 48, IC_SNOOZE);
      break;

    case  734 :
      display->drawXbm(x, y, 48, 48, IC_SORT);
      break;

    case  735 :
      display->drawXbm(x, y, 48, 48, IC_SORT_BY_ALPHA);
      break;

    case  736 :
      display->drawXbm(x, y, 48, 48, IC_SPACE_BAR);
      break;

    case  737 :
      display->drawXbm(x, y, 48, 48, IC_SPA);
      break;

    case  738 :
      display->drawXbm(x, y, 48, 48, IC_SPEAKER);
      break;

    case  739 :
      display->drawXbm(x, y, 48, 48, IC_SPEAKER_GROUP);
      break;

    case  740 :
      display->drawXbm(x, y, 48, 48, IC_SPEAKER_NOTES);
      break;

    case  741 :
      display->drawXbm(x, y, 48, 48, IC_SPEAKER_NOTES_OFF);
      break;

    case  742 :
      display->drawXbm(x, y, 48, 48, IC_SPEAKER_PHONE);
      break;

    case  743 :
      display->drawXbm(x, y, 48, 48, IC_SPELLCHECK);
      break;

    case  744 :
      display->drawXbm(x, y, 48, 48, IC_STARS);
      break;

    case  745 :
      display->drawXbm(x, y, 48, 48, IC_STAR);
      break;

    case  746 :
      display->drawXbm(x, y, 48, 48, IC_STAR_BORDER);
      break;

    case  747 :
      display->drawXbm(x, y, 48, 48, IC_STAR_HALF);
      break;

    case  748 :
      display->drawXbm(x, y, 48, 48, IC_STAY_CURRENT_LANDSCAPE);
      break;

    case  749 :
      display->drawXbm(x, y, 48, 48, IC_STAY_CURRENT_PORTRAIT);
      break;

    case  750 :
      display->drawXbm(x, y, 48, 48, IC_STAY_PRIMARY_LANDSCAPE);
      break;

    case  751 :
      display->drawXbm(x, y, 48, 48, IC_STAY_PRIMARY_PORTRAIT);
      break;

    case  752 :
      display->drawXbm(x, y, 48, 48, IC_STOP);
      break;

    case  753 :
      display->drawXbm(x, y, 48, 48, IC_STOP_SCREEN_SHARE);
      break;

    case  754 :
      display->drawXbm(x, y, 48, 48, IC_STORE);
      break;

    case  755 :
      display->drawXbm(x, y, 48, 48, IC_STORE_MALL_DIRECTORY);
      break;

    case  756 :
      display->drawXbm(x, y, 48, 48, IC_STRAIGHTEN);
      break;

    case  757 :
      display->drawXbm(x, y, 48, 48, IC_STREETVIEW);
      break;

    case  758 :
      display->drawXbm(x, y, 48, 48, IC_STRIKETHROUGH_S);
      break;

    case  759 :
      display->drawXbm(x, y, 48, 48, IC_STYLE);
      break;

    case  760 :
      display->drawXbm(x, y, 48, 48, IC_SUBDIRECTORY_ARROW_LEFT);
      break;

    case  761 :
      display->drawXbm(x, y, 48, 48, IC_SUBDIRECTORY_ARROW_RIGHT);
      break;

    case  762 :
      display->drawXbm(x, y, 48, 48, IC_SUBJECT);
      break;

    case  763 :
      display->drawXbm(x, y, 48, 48, IC_SUBSCRIPTIONS);
      break;

    case  764 :
      display->drawXbm(x, y, 48, 48, IC_SUBTITLES);
      break;

    case  765 :
      display->drawXbm(x, y, 48, 48, IC_SUBWAY);
      break;

    case  766 :
      display->drawXbm(x, y, 48, 48, IC_SUPERVISOR_ACCOUNT);
      break;

    case  767 :
      display->drawXbm(x, y, 48, 48, IC_SURROUND_SOUND);
      break;

    case  768 :
      display->drawXbm(x, y, 48, 48, IC_SWAP_CALLS);
      break;

    case  769 :
      display->drawXbm(x, y, 48, 48, IC_SWAP_HORIZ);
      break;

    case  770 :
      display->drawXbm(x, y, 48, 48, IC_SWAP_VERTICAL_CIRCLE);
      break;

    case  771 :
      display->drawXbm(x, y, 48, 48, IC_SWAP_VERT);
      break;

    case  772 :
      display->drawXbm(x, y, 48, 48, IC_SWITCH_CAMERA);
      break;

    case  773 :
      display->drawXbm(x, y, 48, 48, IC_SWITCH_VIDEO);
      break;

    case  774 :
      display->drawXbm(x, y, 48, 48, IC_SYNC);
      break;

    case  775 :
      display->drawXbm(x, y, 48, 48, IC_SYNC_DISABLED);
      break;

    case  776 :
      display->drawXbm(x, y, 48, 48, IC_SYNC_PROBLEM);
      break;

    case  777 :
      display->drawXbm(x, y, 48, 48, IC_SYSTEM_UPDATE_ALT);
      break;

    case  778 :
      display->drawXbm(x, y, 48, 48, IC_SYSTEM_UPDATE);
      break;

    case  779 :
      display->drawXbm(x, y, 48, 48, IC_TABLET_ANDROID);
      break;

    case  780 :
      display->drawXbm(x, y, 48, 48, IC_TABLET);
      break;

    case  781 :
      display->drawXbm(x, y, 48, 48, IC_TABLET_MAC);
      break;

    case  782 :
      display->drawXbm(x, y, 48, 48, IC_TAB);
      break;

    case  783 :
      display->drawXbm(x, y, 48, 48, IC_TAB_UNSELECTED);
      break;

    case  784 :
      display->drawXbm(x, y, 48, 48, IC_TAG_FACES);
      break;

    case  785 :
      display->drawXbm(x, y, 48, 48, IC_TAP_AND_PLAY);
      break;

    case  786 :
      display->drawXbm(x, y, 48, 48, IC_TERRAIN);
      break;

    case  787 :
      display->drawXbm(x, y, 48, 48, IC_TEXTSMS);
      break;

    case  788 :
      display->drawXbm(x, y, 48, 48, IC_TEXTURE);
      break;

    case  789 :
      display->drawXbm(x, y, 48, 48, IC_TEXT_FIELDS);
      break;

    case  790 :
      display->drawXbm(x, y, 48, 48, IC_TEXT_FORMAT);
      break;

    case  791 :
      display->drawXbm(x, y, 48, 48, IC_THEATERS);
      break;

    case  792 :
      display->drawXbm(x, y, 48, 48, IC_THUMBS_UP_DOWN);
      break;

    case  793 :
      display->drawXbm(x, y, 48, 48, IC_THUMB_DOWN);
      break;

    case  794 :
      display->drawXbm(x, y, 48, 48, IC_THUMB_UP);
      break;

    case  795 :
      display->drawXbm(x, y, 48, 48, IC_TIMELAPSE);
      break;

    case  796 :
      display->drawXbm(x, y, 48, 48, IC_TIMELINE);
      break;

    case  797 :
      display->drawXbm(x, y, 48, 48, IC_TIMER_10);
      break;

    case  798 :
      display->drawXbm(x, y, 48, 48, IC_TIMER_3);
      break;

    case  799 :
      display->drawXbm(x, y, 48, 48, IC_TIMER);
      break;

    case  800 :
      display->drawXbm(x, y, 48, 48, IC_TIMER_OFF);
      break;

    case  801 :
      display->drawXbm(x, y, 48, 48, IC_TIME_TO_LEAVE);
      break;

    case  802 :
      display->drawXbm(x, y, 48, 48, IC_TITLE);
      break;

    case  803 :
      display->drawXbm(x, y, 48, 48, IC_TOC);
      break;

    case  804 :
      display->drawXbm(x, y, 48, 48, IC_TODAY);
      break;

    case  805 :
      display->drawXbm(x, y, 48, 48, IC_TOLL);
      break;

    case  806 :
      display->drawXbm(x, y, 48, 48, IC_TONALITY);
      break;

    case  807 :
      display->drawXbm(x, y, 48, 48, IC_TOUCH_APP);
      break;

    case  808 :
      display->drawXbm(x, y, 48, 48, IC_TOYS);
      break;

    case  809 :
      display->drawXbm(x, y, 48, 48, IC_TRACK_CHANGES);
      break;

    case  810 :
      display->drawXbm(x, y, 48, 48, IC_TRAFFIC);
      break;

    case  811 :
      display->drawXbm(x, y, 48, 48, IC_TRAIN);
      break;

    case  812 :
      display->drawXbm(x, y, 48, 48, IC_TRAM);
      break;

    case  813 :
      display->drawXbm(x, y, 48, 48, IC_TRANSFER_WITHIN_A_STATION);
      break;

    case  814 :
      display->drawXbm(x, y, 48, 48, IC_TRANSFORM);
      break;

    case  815 :
      display->drawXbm(x, y, 48, 48, IC_TRANSLATE);
      break;

    case  816 :
      display->drawXbm(x, y, 48, 48, IC_TRENDING_DOWN);
      break;

    case  817 :
      display->drawXbm(x, y, 48, 48, IC_TRENDING_FLAT);
      break;

    case  818 :
      display->drawXbm(x, y, 48, 48, IC_TRENDING_UP);
      break;

    case  819 :
      display->drawXbm(x, y, 48, 48, IC_TUNE);
      break;

    case  820 :
      display->drawXbm(x, y, 48, 48, IC_TURNED_IN);
      break;

    case  821 :
      display->drawXbm(x, y, 48, 48, IC_TURNED_IN_NOT);
      break;

    case  822 :
      display->drawXbm(x, y, 48, 48, IC_TV);
      break;

    case  823 :
      display->drawXbm(x, y, 48, 48, IC_UNARCHIVE);
      break;

    case  824 :
      display->drawXbm(x, y, 48, 48, IC_UNDO);
      break;

    case  825 :
      display->drawXbm(x, y, 48, 48, IC_UNFOLD_LESS);
      break;

    case  826 :
      display->drawXbm(x, y, 48, 48, IC_UNFOLD_MORE);
      break;

    case  827 :
      display->drawXbm(x, y, 48, 48, IC_UPDATE);
      break;

    case  828 :
      display->drawXbm(x, y, 48, 48, IC_VERIFIED_USER);
      break;

    case  829 :
      display->drawXbm(x, y, 48, 48, IC_VERTICAL_ALIGN_BOTTOM);
      break;

    case  830 :
      display->drawXbm(x, y, 48, 48, IC_VERTICAL_ALIGN_CENTER);
      break;

    case  831 :
      display->drawXbm(x, y, 48, 48, IC_VERTICAL_ALIGN_TOP);
      break;

    case  832 :
      display->drawXbm(x, y, 48, 48, IC_VIBRATION);
      break;

    case  833 :
      display->drawXbm(x, y, 48, 48, IC_VIDEOCAM);
      break;

    case  834 :
      display->drawXbm(x, y, 48, 48, IC_VIDEOCAM_OFF);
      break;

    case  835 :
      display->drawXbm(x, y, 48, 48, IC_VIDEOGAME_ASSET);
      break;

    case  836 :
      display->drawXbm(x, y, 48, 48, IC_VIDEO_CALL);
      break;

    case  837 :
      display->drawXbm(x, y, 48, 48, IC_VIDEO_LABEL);
      break;

    case  838 :
      display->drawXbm(x, y, 48, 48, IC_VIDEO_LIBRARY);
      break;

    case  839 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_AGENDA);
      break;

    case  840 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_ARRAY);
      break;

    case  841 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_CAROUSEL);
      break;

    case  842 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_COLUMN);
      break;

    case  843 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_COMFY);
      break;

    case  844 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_COMPACT);
      break;

    case  845 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_DAY);
      break;

    case  846 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_HEADLINE);
      break;

    case  847 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_LIST);
      break;

    case  848 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_MODULE);
      break;

    case  849 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_QUILT);
      break;

    case  850 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_STREAM);
      break;

    case  851 :
      display->drawXbm(x, y, 48, 48, IC_VIEW_WEEK);
      break;

    case  852 :
      display->drawXbm(x, y, 48, 48, IC_VIGNETTE);
      break;

    case  853 :
      display->drawXbm(x, y, 48, 48, IC_VISIBILITY);
      break;

    case  854 :
      display->drawXbm(x, y, 48, 48, IC_VISIBILITY_OFF);
      break;

    case  855 :
      display->drawXbm(x, y, 48, 48, IC_VOICEMAIL);
      break;

    case  856 :
      display->drawXbm(x, y, 48, 48, IC_VOICE_CHAT);
      break;

    case  857 :
      display->drawXbm(x, y, 48, 48, IC_VOLUME_DOWN);
      break;

    case  858 :
      display->drawXbm(x, y, 48, 48, IC_VOLUME_MUTE);
      break;

    case  859 :
      display->drawXbm(x, y, 48, 48, IC_VOLUME_OFF);
      break;

    case  860 :
      display->drawXbm(x, y, 48, 48, IC_VOLUME_UP);
      break;

    case  861 :
      display->drawXbm(x, y, 48, 48, IC_VPN_KEY);
      break;

    case  862 :
      display->drawXbm(x, y, 48, 48, IC_VPN_LOCK);
      break;

    case  863 :
      display->drawXbm(x, y, 48, 48, IC_WARNING);
      break;

    case  864 :
      display->drawXbm(x, y, 48, 48, IC_WATCH);
      break;

    case  865 :
      display->drawXbm(x, y, 48, 48, IC_WATCH_LATER);
      break;

    case  866 :
      display->drawXbm(x, y, 48, 48, IC_WB_AUTO);
      break;

    case  867 :
      display->drawXbm(x, y, 48, 48, IC_WB_CLOUDY);
      break;

    case  868 :
      display->drawXbm(x, y, 48, 48, IC_WB_INCANDESCENT);
      break;

    case  869 :
      display->drawXbm(x, y, 48, 48, IC_WB_IRIDESCENT);
      break;

    case  870 :
      display->drawXbm(x, y, 48, 48, IC_WB_SUNNY);
      break;

    case  871 :
      display->drawXbm(x, y, 48, 48, IC_WC);
      break;

    case  872 :
      display->drawXbm(x, y, 48, 48, IC_WEB_ASSET);
      break;

    case  873 :
      display->drawXbm(x, y, 48, 48, IC_WEB);
      break;

    case  874 :
      display->drawXbm(x, y, 48, 48, IC_WEEKEND);
      break;

    case  875 :
      display->drawXbm(x, y, 48, 48, IC_WHATSHOT);
      break;

    case  876 :
      display->drawXbm(x, y, 48, 48, IC_WIFI);
      break;

    case  877 :
      display->drawXbm(x, y, 48, 48, IC_WORK);
      break;

    case  878 :
      display->drawXbm(x, y, 48, 48, IC_WRAP_TEXT);
      break;

    case  879 :
      display->drawXbm(x, y, 48, 48, IC_YOUTUBE_SEARCHED_FOR);
      break;

    case  880 :
      display->drawXbm(x, y, 48, 48, IC_ZOOM_IN);
      break;

    case  881 :
      display->drawXbm(x, y, 48, 48, IC_ZOOM_OUT);
      break;

    case  882 :
      display->drawXbm(x, y, 48, 48, IC_ZOOM_OUT_MAP);
      break;

    default:
      // statements
      display->drawXbm(x, y, 48, 48, ICON_NOT_FOUND);
      break;
  }
}


void drawFrame1(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  ui.disableAllIndicators();
  drawProgress(display, x, y, Page1Title, Page1Subtext.toInt(), Page1Icon);
}

void drawFrame2(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  ui.enableAllIndicators();
  drawScreen(display, x, y, Page2Title, Page2Subtext, Page2Icon);
}

void drawFrame3(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  drawScreen(display, x, y, Page3Title, Page3Subtext,  Page3Icon);
}

void drawFrame4(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  drawScreen(display, x, y, Page4Title, Page4Subtext,  Page4Icon);
}

void drawFrame5(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {
  drawScreen(display, x, y, Page5Title, Page5Subtext,  Page5Icon);
}


// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, drawFrame4, drawFrame5};
int numberOfFrames = 5;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(255);


  // initialize dispaly
  display.init();
  display.clear();
  display.display();
  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);

  display.clear();
  display.drawXbm(0, 0, Startup_Logo_width, Startup_Logo_height, Startup_Logo);
  display.display();
  delay(1000);


  delay(10);
  // We start by connecting to a WiFi network
  Serial.print("\nConnecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(250);
    Serial.print(".");
    display.clear();
    display.drawString(64, 10, "Connecting to WiFi");
    display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbol : inactiveSymbol);
    display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbol : inactiveSymbol);
    display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbol : inactiveSymbol);
    display.display();
    counter++;

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
    }
    FastLED.show();
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();


  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ui.setTargetFPS(30);
  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, numberOfFrames);
  ui.init();
  Serial.println("");
}

void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {

    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    //    leds[0].setHSV(LED1_H, LED1_S, LED1_V);
    //    leds[1].setHSV(LED2_H, LED2_S, LED2_V);
    //    leds[2].setHSV(LED3_H, LED3_S, LED3_V);
    //    leds[3].setHSV(LED4_H, LED4_S, LED4_V);
    //    leds[4].setHSV(LED5_H, LED5_S, LED5_V);
    //
    //    FastLED.show();
    delay(remainingTimeBudget);
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= flash_interval) {
    previousMillis = currentMillis;

    if (flash_on == LOW) {
      flash_on = HIGH;
      for (int i = 0; i <= 5; i++) {
        if (FlashArray[i] == true) {
          leds[i] = CRGB::Black;
        }
      }
    } else {
      flash_on = LOW;
      leds[4].setHSV(LED1_H, LED1_S, LED1_V);
      leds[3].setHSV(LED2_H, LED2_S, LED2_V);
      leds[2].setHSV(LED3_H, LED3_S, LED3_V);
      leds[1].setHSV(LED4_H, LED4_S, LED4_V);
      leds[0].setHSV(LED5_H, LED5_S, LED5_V);
    }
    FastLED.show();
  }
}
