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
-- Table structure for table `transaction`
--

DROP TABLE IF EXISTS `transaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transaction` (
  `Transaction_ID` int NOT NULL,
  `Buyer_ID` int NOT NULL,
  `Seller_ID` int NOT NULL,
  `Item_ID` int NOT NULL,
  `Amount` decimal(10,2) NOT NULL,
  `Fee` decimal(10,2) NOT NULL,
  `Payment_Method` varchar(45) NOT NULL,
  `Buyer_Address` text NOT NULL,
  `Transaction_Date` date NOT NULL,
  `Transaction_Status` enum('Pending','Completed','Canceled') NOT NULL DEFAULT 'Pending',
  PRIMARY KEY (`Transaction_ID`),
  KEY `FK_Buyer_idx` (`Buyer_ID`),
  KEY `FK_Seller_idx` (`Seller_ID`),
  KEY `FK_ItemID_idx` (`Item_ID`),
  CONSTRAINT `FK_Buyer` FOREIGN KEY (`Buyer_ID`) REFERENCES `user` (`User_ID`),
  CONSTRAINT `FK_Itemid` FOREIGN KEY (`Item_ID`) REFERENCES `item` (`Item_ID`),
  CONSTRAINT `FK_Seller` FOREIGN KEY (`Seller_ID`) REFERENCES `user` (`User_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transaction`
--

LOCK TABLES `transaction` WRITE;
/*!40000 ALTER TABLE `transaction` DISABLE KEYS */;
INSERT INTO `transaction` VALUES (40001,10001,10005,30020,40.00,1.20,'TNG','123 Main St, KL','2024-01-26','Completed'),(40002,10001,10006,30016,120.00,3.60,'Credit Card','123 Main St, KL','2024-01-26','Completed'),(40003,10006,10007,30012,30.00,0.90,'Credit Card','22 Rose Street, Ipoh','2024-02-20','Completed'),(40004,10006,10005,30008,500.00,15.00,'Credit Card','22 Rose Street, Ipoh','2024-02-27','Completed'),(40005,10001,10007,30019,180.00,5.40,'Credit Card','123 Main St, KL','2024-03-27','Completed'),(40006,10001,10005,30015,350.00,10.50,'Credit Card','123 Main St, KL','2024-03-27','Completed'),(40007,10001,10003,30003,200.00,6.00,'Credit Card','123 Main St, KL','2024-04-27','Completed'),(40008,10001,10003,30007,150.00,4.50,'Credit Card','123 Main St, KL','2024-04-27','Completed'),(40009,10001,10002,30018,300.00,9.00,'TNG','123 Main St, KL','2024-04-27','Completed'),(40010,10004,10003,30022,15.00,0.45,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2024-05-03','Completed'),(40012,10003,10006,30010,250.00,7.50,'TNG','8 Green Road, Melaka','2024-05-05','Pending'),(40013,10003,10004,30006,600.00,18.00,'TNG','8 Green Road, Melaka','2024-05-06','Completed'),(40014,10004,10001,30001,15.50,0.47,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2024-06-02','Completed'),(40015,10004,10017,30041,15.00,0.45,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2024-10-10','Completed'),(40016,10007,10009,30042,600.00,18.00,'Credit Card','32 Park Lane, Penang','2024-06-19','Completed'),(40017,10012,10010,30043,80.00,2.40,'TNG','55 Pine Street, Melaka','2024-07-01','Completed'),(40018,10010,10012,30044,50.00,1.50,'Credit Card','77 Seaside Road, KL','2024-07-11','Pending'),(40019,10006,10014,30045,40.00,1.20,'Credit Card','22 Rose Street, Ipoh','2024-07-15','Pending'),(40020,10011,10013,30046,20.00,0.60,'TNG','11 City Centre, Penang','2024-07-15','Completed'),(40021,10010,10012,30047,100.00,3.00,'Credit Card','77 Seaside Road, KL','2024-07-20','Pending'),(40022,10017,10011,30048,30.00,0.90,'TNG','3 Central Park, KL','2024-08-08','Completed'),(40023,10014,10012,30049,120.00,3.60,'Credit Card','39 Hilltop Road, KL','2024-08-08','Pending'),(40024,10012,10017,30050,45.00,1.35,'TNG','55 Pine Street, Melaka','2024-08-08','Pending'),(40025,10016,10013,30051,60.00,1.80,'Credit Card','66 Riverside Drive, Penang','2024-08-18','Completed'),(40026,10013,10014,30052,35.00,1.05,'TNG','18 Lakeside Drive, Johor','2024-09-02','Completed'),(40027,10017,10016,30053,150.00,4.50,'Credit Card','3 Central Park, KL','2024-09-10','Completed'),(40028,10014,10012,30054,25.00,0.75,'TNG','39 Hilltop Road, KL','2024-10-07','Completed'),(40029,10012,10016,30055,40.00,1.20,'Credit Card','55 Pine Street, Melaka','2024-10-09','Pending'),(40030,10017,10013,30056,75.00,2.25,'TNG','3 Central Park, KL','2024-10-19','Pending'),(40031,10013,10014,30057,100.00,3.00,'Credit Card','18 Lakeside Drive, Johor','2024-10-25','Completed'),(40032,10009,10015,30058,200.00,6.00,'Credit Card','50 Mountain Top, Johor','2024-10-30','Completed'),(40033,10012,10010,30059,30.00,0.90,'TNG','55 Pine Street, Melaka','2024-11-11','Completed'),(40034,10014,10011,30060,25.00,0.75,'Credit Card','39 Hilltop Road, KL','2024-11-11','Completed'),(40035,10011,10016,30061,100.00,3.00,'Credit Card','11 City Centre, Penang','2024-11-15','Pending'),(40036,10009,10017,30062,45.00,1.35,'TNG','50 Mountain Top, Johor','2024-11-20','Completed'),(40037,10012,10010,30063,50.00,1.50,'Credit Card','55 Pine Street, Melaka','2024-11-29','Pending'),(40038,10006,10009,30064,90.00,2.70,'TNG','22 Rose Street, Ipoh','2024-12-02','Completed'),(40039,10011,10013,30065,200.00,6.00,'Credit Card','11 City Centre, Penang','2024-12-02','Completed'),(40040,10012,10015,30066,40.00,1.20,'TNG','55 Pine Street, Melaka','2024-12-03','Pending'),(40041,10016,10017,30067,150.00,4.50,'Credit Card','66 Riverside Drive, Penang','2024-12-06','Completed'),(40042,10010,10011,30068,20.00,0.60,'TNG','77 Seaside Road, KL','2024-12-11','Pending'),(40043,10013,10016,30069,30.00,0.90,'Credit Card','18 Lakeside Drive, Johor','2024-12-13','Completed'),(40044,10017,10014,30070,60.00,1.80,'TNG','3 Central Park, KL','2024-12-15','Pending'),(40045,10011,10009,30071,100.00,3.00,'Credit Card','11 City Centre, Penang','2024-12-17','Pending'),(40046,10012,10014,30072,40.00,1.20,'TNG','55 Pine Street, Melaka','2024-12-20','Pending'),(40047,10016,10011,30073,50.00,1.50,'Credit Card','66 Riverside Drive, Penang','2024-12-23','Pending'),(40048,10013,10017,30074,20.00,0.60,'TNG','18 Lakeside Drive, Johor','2024-12-25','Pending'),(40049,10009,10012,30075,150.00,4.50,'Credit Card','50 Mountain Top, Johor','2024-12-27','Pending'),(40050,10011,10016,30076,350.00,10.50,'TNG','11 City Centre, Penang','2024-12-27','Pending'),(40051,10001,10018,30038,20.00,0.60,'TNG','123 Main St, KL','2024-12-27','Completed'),(40052,10001,10017,30040,15.00,0.45,'TNG','123 Main St, KL','2024-12-27','Completed'),(40053,10004,10012,30023,60.00,1.80,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2024-12-27','Completed'),(40054,10004,10010,30083,50.00,1.50,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2024-12-29','Completed'),(40055,10004,10013,30090,20.00,0.60,'Credit Card','Satria Jebat, Kediaman UTeM, Melaka','2024-12-30','Pending'),(40056,10004,10009,30032,10.00,0.30,'Credit Card','Satria Jebat, Kediaman UTeM, Melaka','2025-01-01','Pending'),(40057,10004,10013,30039,15.00,0.45,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2025-01-02','Pending'),(40058,10004,10013,30029,15.00,0.45,'Credit Card','Satria Jebat, Kediaman UTeM, Melaka','2025-01-03','Pending'),(40059,10004,10014,30079,30.00,0.90,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2025-01-07','Pending'),(40060,10031,10018,30085,120.00,3.60,'TNG','28 Hilltop Road, KL','2025-01-07','Pending'),(40061,10031,10013,30087,100.00,3.00,'TNG','28 Hilltop Road, KL','2025-01-07','Pending'),(40062,10031,10017,30078,20.00,0.60,'TNG','28 Hilltop Road, KL','2025-01-07','Pending'),(40064,10031,10010,30024,50.00,1.50,'TNG','28 Hilltop Road, KL','2025-01-07','Pending'),(40065,10031,10011,30093,50.00,1.50,'Credit Card','28 Hilltop Road, KL','2025-01-07','Pending'),(40066,10031,10011,30088,40.00,1.20,'TNG','28 Hilltop Road, KL','2025-01-07','Pending'),(40067,10004,10031,30094,650.00,19.50,'TNG','Satria Jebat, Kediaman UTeM, Melaka','2025-01-08','Completed'),(40069,10001,10009,30082,60.00,1.80,'TNG','123 Main St, KL','2025-01-08','Pending'),(40070,10032,10001,30004,100.00,3.00,'TNG','Satria Tuah, Kediaman UTeM, Melaka','2025-01-08','Completed'),(40073,10032,10014,30092,250.00,7.50,'TNG','Satria Tuah, Kediaman UTeM, Melaka','2025-01-11','Completed'),(40074,10032,10012,30089,40.00,1.20,'TNG','Satria Tuah, Kediaman UTeM, Melaka','2025-01-11','Pending'),(40075,10001,10002,30002,1200.00,36.00,'TNG','123 Main St, KL','2025-01-18','Pending'),(40076,10033,10032,30096,5.00,0.15,'TNG','ttttttttttttttt','2025-01-20','Completed'),(40077,10001,10033,30097,14.00,0.42,'TNG','123 Main St, KL','2025-01-20','Completed'),(40078,10034,10016,30034,40.00,1.20,'TNG','Satria Tuah, Durian Tunggal','2025-01-20','Completed'),(40079,10001,10034,30098,25.00,0.75,'TNG','123 Main St, KL','2025-01-20','Pending');
/*!40000 ALTER TABLE `transaction` ENABLE KEYS */;
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
