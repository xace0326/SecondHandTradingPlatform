-- MySQL dump 10.13  Distrib 8.0.40, for Win64 (x86_64)
--
-- Host: localhost    Database: second-hand_trading_platform
-- ------------------------------------------------------
-- Server version	8.0.40

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user` (
  `User_ID` int NOT NULL,
  `Username` varchar(50) NOT NULL,
  `Password` varchar(255) NOT NULL,
  `Email` varchar(100) NOT NULL,
  `Phone` varchar(15) NOT NULL,
  `Name` varchar(100) NOT NULL,
  `Address` text NOT NULL,
  PRIMARY KEY (`User_ID`),
  UNIQUE KEY `USERNAME_UNIQUE` (`Username`),
  UNIQUE KEY `PHONE_UNIQUE` (`Phone`),
  UNIQUE KEY `EMAIL_UNIQUE` (`Email`),
  UNIQUE KEY `User_ID_UNIQUE` (`User_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (10001,'soonweihong','q1w2e3r4','soonweihong0857@gmail.com','0169695272','Soon Wei Hong','123 Main St, KL'),(10002,'yang','q1w2e3r4','yang@gmail.com','0121212345','Tay Sher Yang','45 Sunset Blvd, Penang'),(10003,'Liew','q1w2e3r4','liewkhengyip@gmail.com','0162947472','Liew Kheng Yip','8 Green Road, Melaka'),(10004,'Jun','12345','Jun@gmail.com','01232345667','Jun Teoh','Satria Jebat, Kediaman UTeM, Melaka'),(10005,'jason123','54321','jason123@gmail.com','0154325837','Jason Choung','67 Ocean Drive, Johor'),(10006,'opchannel','Q!w2e3r4','opchannel@gmail.com','01745627285','Channel OP','22 Rose Street, Ipoh'),(10007,'alex98','A1b2c3d4','alex98@gmail.com','0167896543','Alex Wong','32 Park Lane, Penang'),(10008,'michelle88','M1n2o3p4','michelle88@gmail.com','0123456789','Michelle Tan','12 River View, KL'),(10009,'kevin_thepro','K!p2o3l4','kevinpro@gmail.com','0112233445','Kevin Lim','50 Mountain Top, Johor'),(10010,'rachel_star','R@t4s5t6','rachel.star@gmail.com','0198765432','Rachel Chew','77 Seaside Road, KL'),(10011,'benjamin91','B7n8m9j1','benjamin91@gmail.com','0147891236','Benjamin Ng','11 City Centre, Penang'),(10012,'chloelee','C!h2l3e4','chloe.lee@gmail.com','0185678910','Chloe Lee','55 Pine Street, Melaka'),(10013,'daniel_rich','D4a5n6r7','danielrich@gmail.com','0176543210','Daniel Rich','18 Lakeside Drive, Johor'),(10014,'sophia95','S@p6h7i8','sophia95@gmail.com','0129876543','Sophia Lau','39 Hilltop Road, KL'),(10015,'ethan99','E9t8h7a6','ethan99@gmail.com','0161122334','Ethan Ong','13 Country Road, Ipoh'),(10016,'lily_blossom','L!b2o3m4','lily.blossom@gmail.com','0194455667','Lily Blossom','66 Riverside Drive, Penang'),(10017,'mark_king','M2a3r4k5','markking@gmail.com','0117788990','Mark King','3 Central Park, KL'),(10018,'grace_tan','G@r3a4t5','grace.tan@gmail.com','0183344556','Grace Tan','25 Bayfront Avenue, Johor'),(10019,'john_doe','J#d4o5e6','johndoe@gmail.com','0125566778','John Doe','5 Forest Road, Melaka'),(10020,'clara_white','C7l8a9r0','clara.white@gmail.com','0167788991','Clara White','10 Beach Street, KL'),(10021,'alice_wong','A1l2i3c4','alice.wong@gmail.com','0161234567','Alice Wong','19 Greenfield Road, Penang'),(10022,'johnsmith','J#s5m6i7','john.smith@gmail.com','0149876543','John Smith','44 Sunset Lane, Ipoh'),(10023,'lucas_lee','L!u9c8s0','lucas.lee@gmail.com','0172345678','Lucas Lee','8 Riverbend Drive, Johor'),(10024,'emilyjones','E8m9i0y2','emily.jones@gmail.com','0187654321','Emily Jones','33 Maple Road, KL'),(10025,'jackie_chan','Jc!a1k2e3','jackie.chan@gmail.com','0168765432','Jackie Chan','20 Palm Street, Penang'),(10026,'peterparker','P!e2t3r4','peter.parker@gmail.com','0162345678','Peter Parker','15 Star Avenue, Ipoh'),(10027,'sarah_choo','S7a6r5h8','sarah.choo@gmail.com','0178765432','Sarah Choo','40 Greenfield Road, Johor'),(10028,'tom_hardy','T0m1H2r3','tom.hardy@gmail.com','0192345678','Tom Hardy','24 Ocean Blvd, KL'),(10029,'emma_stone','E2m3m4a5','emma.stone@gmail.com','0119876543','Emma Stone','14 Birchwood Drive, Penang'),(10030,'robertpatt','R!o9b8e7','robert.patt@gmail.com','0123455789','Robert Patt','18 Sunset Blvd, Johor'),(10031,'xace0326','q1w2e3r4','xace0326@gmail.com','0183758382','Ace Wang','28 Hilltop Road, KL'),(10032,'soon','54321','soon@gmail.com','0169695222','SoonUTeM','Satria Tuah, Kediaman UTeM, Melaka'),(10033,'test','123','test@gmail.com','0159695272','tezst','Satria Tuah Durian Tunggal'),(10034,'testing123','123','testing123@gmail.com','0123234523','Test','Satria Tuah, Durian Tunggal'),(10035,'testing321','123','testing321@gmail.com','01232323233','TEST ONE','Satria Tuah, Kediaman UTeM, Melaka');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-06-13 16:44:33
