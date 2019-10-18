/*
Navicat MySQL Data Transfer

Source Server         : MySQL
Source Server Version : 80017
Source Host           : localhost:3306
Source Database       : virus4secguard

Target Server Type    : MYSQL
Target Server Version : 80017
File Encoding         : 65001

Date: 2019-10-18 08:43:56
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for virusinfo
-- ----------------------------
DROP TABLE IF EXISTS `virusinfo`;
CREATE TABLE `virusinfo` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `md5` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of virusinfo
-- ----------------------------
INSERT INTO `virusinfo` VALUES ('1', 'C:\\Users\\Rylynn\\source\\repos\\SecurityGuard\\SecurityGuard\\res\\AntiVirus\\KillOnline\\virus1.txt', '-348644864-2063922425-2059011273-348644864');
INSERT INTO `virusinfo` VALUES ('2', 'C:\\Users\\Rylynn\\source\\repos\\SecurityGuard\\SecurityGuard\\res\\AntiVirus\\KillOnline\\virus2.txt', '-1930079617-471465034-231913066-1930079617');
