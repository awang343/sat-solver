import re

def parse_dimacs(dimacs_file):
    clauses = []
    with open(dimacs_file, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('c') or line.startswith('p'):
                continue
            clause = [int(x) for x in line.split() if x != '0']
            if clause:
                clauses.append(clause)
    return clauses

def parse_solution(solution_str):
    assignments = {}
    tokens = solution_str.split()
    for i in range(0, len(tokens), 2):
        var = int(tokens[i])
        value = tokens[i + 1] == 'true'
        assignments[var] = value
    return assignments

def check_satisfaction(dimacs_file, solution_str):
    clauses = parse_dimacs(dimacs_file)
    assignments = parse_solution(solution_str)
    
    for clause in clauses:
        satisfied = False
        for literal in clause:
            var = abs(literal)
            if var in assignments:
                if (literal > 0 and assignments[var]) or (literal < 0 and not assignments[var]):
                    satisfied = True
                    break

        if not satisfied:
            print(clause)
            print([assignments[abs(literal)] for literal in clause])
            return False
    return True

# Example Usage
data = {"Instance": "C140.cnf", "Time": 0.85, "Result": "SAT", "Solution": "108 true 669 true 577 true 328 true 563 true 280 true 566 true 627 true 186 true 351 true 360 true 449 true 443 true 447 true 444 true 441 true 436 true 607 true 616 true 617 true 358 true 355 false 425 true 567 true 421 true 508 true 517 true 279 true 70 true 243 false 260 true 261 true 256 true 255 false 251 true 246 false 274 true 191 false 437 true 432 true 433 true 327 true 312 true 228 true 200 true 199 true 224 true 311 true 190 true 227 true 310 true 307 true 306 true 234 true 205 true 214 true 309 true 308 true 201 true 277 true 204 true 208 true 211 true 305 true 235 true 237 true 198 true 193 true 196 true 233 true 225 true 164 true 163 true 160 true 162 true 41 true 26 true 18 true 16 true 15 true 19 true 159 false 262 true 440 true 154 true 148 true 149 true 146 true 147 true 562 true 605 true 570 true 125 true 124 true 123 true 127 true 134 true 128 true 129 true 122 false 121 false 120 false 119 false 118 false 117 false 13 false 380 true 445 true 555 true 448 true 330 true 326 true 183 true 271 true 259 true 268 true 141 true 215 true 138 true 137 true 651 true 378 true 505 true 381 true 140 true 509 true 1 true 385 true 673 true 377 true 135 true 406 true 411 true 405 true 674 true 409 true 446 true 83 true 85 true 87 true 407 true 408 true 413 true 434 true 178 true 650 true 672 true 671 true 683 true 69 true 687 true 688 true 684 true 628 true 626 true 623 true 474 true 652 true 278 true 179 true 270 true 184 true 414 true 353 true 185 true 702 true 8 true 5 true 71 true 68 true 565 true 72 true 153 true 195 true 188 true 7 true 3 true 290 true 398 true 187 true 6 true 2 true 680 true 667 true 442 true 289 true 606 true 132 true 4 true 113 true 111 true 130 true 131 true 112 true 116 true 126 true 29 true 554 true 552 true 548 true 538 true 541 true 27 true 540 true 547 true 28 true 534 true 568 true 343 true 399 true 412 true 428 true 341 true 340 true 91 true 221 true 387 true 133 true 388 true 88 true 89 true 90 true 218 true 417 true 220 true 219 true 167 true 81 true 342 true 808 true 810 true 816 true 817 true 822 true 823 true 827 true 829 true 832 true 833 true 834 true 835 true 839 true 840 true 841 true 842 true 843 true 844 true 845 true 854 true 855 true 856 true 857 true 858 true 859 true 860 true 861 true 862 true 863 true 864 true 865 true 866 true 783 true 870 true 781 true 572 true 655 true 642 true 830 true 641 true 871 true 578 true 599 true 581 true 580 true 576 true 575 true 574 true 573 true 571 true 332 true 564 true 853 true 551 true 550 true 549 true 852 true 851 true 546 true 850 true 849 true 848 true 539 true 847 true 535 true 846 true 813 true 814 true 615 true 166 true 831 true 331 true 373 true 376 true 181 true 165 true 422 true 618 true 872 true 879 true 807 true 633 true 367 true 634 true 395 true 394 true 82 true 170 true 828 true 222 true 880 true 67 true 826 true 217 true 629 true 815 true 569 true 285 true 825 true 284 true 172 true 430 true 824 true 283 true 811 true 80 true 776 true 894 true 662 true 890 true 869 true 84 true 545 true 663 true 780 true 614 true 630 true 657 true 658 true 664 true 238 true 896 true 785 true 668 true 897 true 786 true 787 true 386 true 882 true 885 true 891 true 892 true 893 true 895 true 419 true 266 true 809 true 812 true 818 true 819 true 544 true 820 true 821 true 287 true 507 true 836 true 295 true 561 true 837 true 296 true 838 true 297 true 883 true 463 true 464 true 467 true 867 true 86 true 431 true 560 true 426 true 438 true 429 true 439 true 423 true 420 true 415 true 401 true 396 true 806 true 805 true 804 true 803 true 802 true 801 true 800 true 799 true 798 true 797 true 796 true 795 true 794 true 793 true 792 true 791 true 790 true 789 true 788 true 784 true 288 true 868 true 873 true 874 true 875 true 876 true 877 true 878 true 881 true 946 true 945 true 944 true 943 true 942 true 941 true 940 true 939 true 938 true 937 true 936 true 935 true 934 true 933 true 932 true 931 true 930 true 929 true 928 true 927 true 926 true 925 true 924 true 923 true 922 true 921 true 920 true 919 true 918 true 917 true 916 true 915 true 914 true 913 true 912 true 911 true 910 true 909 true 908 true 907 true 906 true 905 true 904 true 903 true 902 true 901 true 900 true 899 true 898 true 889 true 888 true 887 true 886 true 884 true 656 true 648 true 947 true 948 true 949 true 950 true 951 true 952 true 953 true 954 true 955 true 956 true 957 true 958 true 959 true 960 true 961 true 962 true 963 true 964 true 965 true 966 true 967 true 968 true 969 true 970 true 971 true 972 true 973 true 974 true 975 true 976 true 977 true 978 true 979 true 980 true 981 true 982 true 983 true 984 true 985 true 986 true 987 true 988 true 989 true 990 true 991 true 992 true 993 true 994 true 995 true 996 true 997 true 998 true 999 true 1000 true 143 true 145 true 152 true 158 true 161 true 171 true 1003 true 1002 true 1001 true 173 true 1004 true 174 true 175 true 176 true 177 true 180 true 559 true 1841 true 1840 true 1839 true 1838 true 1837 true 1836 true 1835 true 1834 true 1833 true 1832 true 1831 true 1830 true 1829 true 1828 true 1827 true 1826 true 1825 true 1824 true 1823 true 1822 true 1821 true 1820 true 1819 true 1818 true 1817 true 1816 true 1815 true 1814 true 1813 true 1812 true 1811 true 1810 true 1809 true 1808 true 1807 true 1806 true 1805 true 1804 true 1325 true 1320 true 1319 true 210 true 1317 true 1316 true 207 true 1310 true 1309 true 1307 true 1306 true 197 true 1305 true 1304 true 1303 true 194 true 1302 true 1301 true 192 true 1300 true 1299 true 1298 true 189 true 1297 true 1296 true 1293 true 1290 true 1287 true 1286 true 1283 true 1282 true 1281 true 1280 true 1278 true 169 true 1277 true 168 true 1275 true 1273 true 1272 true 1270 true 1269 true 1268 true 1267 true 1266 true 157 true 1265 true 156 true 1263 true 1262 true 1261 true 1260 true 151 true 1259 true 150 true 1258 true 1257 true 1256 true 1255 true 1254 true 1253 true 144 true 1252 true 1250 true 1249 true 1248 true 139 true 1247 true 1246 true 1245 true 136 true 1244 true 1243 true 1242 true 1241 true 1240 true 1236 true 1235 true 1234 true 1231 true 1230 true 1229 true 1228 true 1225 true 1223 true 114 true 1222 true 1220 true 1219 true 110 true 1217 true 1216 true 1215 true 1214 true 1213 true 1212 true 1211 true 1210 true 1209 true 1208 true 1207 true 1205 true 1204 true 1203 true 1199 true 1198 true 1193 true 1189 true 1183 true 1182 true 1175 true 1174 true 1173 true 1172 true 1171 true 435 true 1544 true 1233 true 1327 true 245 true 1354 true 1170 true 1326 true 244 true 1353 true 1169 true 1237 true 155 true 1264 true 1168 true 1239 true 1238 true 349 true 1458 true 348 true 1457 true 250 true 1359 true 249 true 1358 true 325 true 1434 true 347 true 1456 true 1167 true 1165 true 1190 true 649 true 1758 true 346 true 1455 true 427 true 1536 true 1194 true 653 true 1762 true 1164 true 258 true 1367 true 1163 true 424 true 1533 true 339 true 1448 true 247 true 1356 true 1224 true 115 true 142 true 1251 true 248 true 1357 true 1159 true 1206 true 665 true 1774 true 1158 true 418 true 1527 true 257 true 1366 true 1157 true 1156 true 1155 true 1324 true 242 true 1351 true 1323 true 241 true 1350 true 1322 true 213 true 240 true 1349 true 1321 true 212 true 239 true 1348 true 1318 true 209 true 236 true 1345 true 416 true 1525 true 410 true 1519 true 1295 true 1148 true 39 true 1181 true 640 true 1749 true 1180 true 639 true 1748 true 1179 true 638 true 1747 true 1178 true 637 true 1746 true 1177 true 636 true 1745 true 1176 true 635 true 1744 true 1147 true 38 true 404 true 1513 true 403 true 1512 true 1146 true 37 true 1221 true 1145 true 36 true 402 true 1511 true 400 true 1509 true 1144 true 35 true 602 true 1711 true 1143 true 34 true 1294 true 313 true 1422 true 397 true 1506 true 304 true 1413 true 303 true 1412 true 302 true 1411 true 301 true 1410 true 1197 true 300 true 1409 true 1140 true 31 true 1139 true 30 true 597 true 1706 true 1138 true 596 true 1705 true 1137 true 1162 true 621 true 1730 true 1161 true 620 true 1729 true 1160 true 619 true 1728 true 595 true 1704 true 1136 true 1151 true 610 true 1719 true 1150 true 609 true 1718 true 594 true 1703 true 1135 true 593 true 1702 true 1134 true 25 true 1154 true 613 true 1722 true 1149 true 40 true 608 true 1717 true 393 true 1502 true 392 true 1501 true 354 true 1463 true 352 true 1461 true 391 true 1500 true 1153 true 612 true 1721 true 1152 true 611 true 1720 true 333 true 1442 true 1131 true 22 true 590 true 1699 true 298 true 1407 true 525 true 1634 true 1066 true 1065 true 1064 true 1063 true 292 true 1401 true 1062 true 291 true 1400 true 518 true 1627 true 1059 true 1058 true 286 true 1395 true 1056 true 1055 true 1061 true 299 true 1408 true 182 true 1291 true 1054 true 1052 true 281 true 1390 true 344 true 1453 true 1142 true 33 true 601 true 1710 true 1115 true 335 true 1444 true 24 true 1133 true 1106 true 592 true 1701 true 462 true 1571 true 1276 true 1313 true 231 true 1340 true 334 true 1443 true 1132 true 23 true 591 true 1700 true 461 true 1570 true 1274 true 369 true 1478 true 1051 true 1105 true 1050 true 1104 true 1049 true 1048 true 1279 true 324 true 1433 true 1095 true 506 true 1615 true 1047 true 1046 true 1308 true 226 true 1335 true 1271 true 1070 true 529 true 1638 true 1202 true 661 true 1770 true 1069 true 528 true 1637 true 1201 true 660 true 1769 true 1041 true 1068 true 527 true 1636 true 1039 true 1015 true 1042 true 501 true 1610 true 1022 true 314 true 1423 true 1085 true 473 true 1582 true 1014 true 1021 true 1020 true 1019 true 1018 true 1045 true 504 true 1613 true 472 true 1581 true 1013 true 1040 true 269 true 1378 true 1200 true 659 true 1768 true 1032 true 1016 true 1043 true 502 true 1611 true 471 true 1580 true 1012 true 372 true 1481 true 1191 true 109 true 1218 true 366 true 1475 true 1008 true 526 true 1635 true 1067 true 1227 true 686 true 1795 true 370 true 1479 true 1005 true 1311 true 202 true 229 true 1338 true 356 true 1465 true 371 true 1480 true 1196 true 1312 true 203 true 230 true 1339 true 357 true 1466 true 519 true 1628 true 1060 true 1017 true 1044 true 503 true 1612 true 368 true 1477 true 1166 true 625 true 1734 true 350 true 1459 true 1121 true 12 true 1053 true 282 true 1391 true 1023 true 1184 true 643 true 1752 true 1024 true 1185 true 644 true 1753 true 1037 true 1025 true 1186 true 645 true 1754 true 1038 true 1026 true 1187 true 646 true 1755 true 389 true 1498 true 1314 true 232 true 1341 true 773 true 1030 true 1057 true 516 true 1625 true 1027 true 1188 true 647 true 1756 true 390 true 1499 true 1315 true 206 true 774 true 1031 true 1028 true 1029 true 1033 true 1034 true 470 true 1579 true 1011 true 329 true 1438 true 1100 true 1127 true 586 true 1695 true 1035 true 1036 true 336 true 1445 true 1107 true 1226 true 685 true 1794 true 337 true 1446 true 1108 true 338 true 1447 true 1109 true 530 true 1639 true 1071 true 531 true 1640 true 1072 true 532 true 1641 true 1073 true 533 true 1642 true 1074 true 1075 true 1195 true 654 true 1763 true 1192 true 1076 true 1077 true 1078 true 359 true 1468 true 1079 true 1080 true 1284 true 1285 true 1081 true 1082 true 542 true 1651 true 1083 true 1084 true 1086 true 1087 true 1288 true 1289 true 1007 true 466 true 1575 true 1088 true 1292 true 1089 true 374 true 1483 true 1090 true 9 true 1118 true 1091 true 10 true 1119 true 1092 true 11 true 1120 true 1093 true 1006 true 465 true 1574 true 1094 true 14 true 1123 true 1096 true 1097 true 1232 true 1098 true 17 true 1126 true 1099 true 1101 true 1128 true 587 true 1696 true 20 true 1129 true 1102 true 375 true 1484 true 21 true 1130 true 1103 true 589 true 1698 true 379 true 1488 true 1110 true 382 true 1491 true 1141 true 32 true 600 true 1709 true 1114 true 1111 true 1112 true 1113 true 1009 true 468 true 1577 true 383 true 1492 true 361 true 1470 true 362 true 1471 true 363 true 1472 true 384 true 1493 true 1116 true 345 true 1454 true 1117 true 1010 true 469 true 1578 true 364 true 1473 true 365 true 1474 true 579 true 1688 true 1122 true 582 true 1691 true 583 true 1692 true 1124 true 584 true 1693 true 1125 true 585 true 1694 true 588 true 1697 true 1328 true 1329 true 1330 true 1331 true 1332 true 1333 true 1334 true 1336 true 1337 true 1342 true 1343 true 1344 true 1346 true 1347 true 1352 true 1355 true 1360 true 1361 true 252 true 1362 true 253 true 1363 true 254 true 1364 true 1365 true 1368 true 1369 true 1370 true 1371 true 1372 true 1373 true 1374 true 1375 true 1376 true 1377 true 1379 true 1380 true 1381 true 1382 true 1383 true 1384 true 1385 true 1386 true 1387 true 1388 true 1389 true 1392 true 1393 true 1394 true 1396 true 1397 true 1398 true 1399 true 1402 true 1403 true 1404 true 1405 true 1406 true 1414 true 1415 true 1416 true 1417 true 1418 true 1419 true 1420 true 1421 true 1424 true 1425 true 1426 true 1427 true 1428 true 1429 true 1430 true 1431 true 1432 true 1435 true 1436 true 1437 true 1439 true 1440 true 1441 true 1449 true 1450 true 1451 true 1452 true 1460 true 1462 true 1464 true 1467 true 1469 true 1476 true 1482 true 1485 true 1486 true 1487 true 689 true 1798 true 690 true 1799 true 1489 true 1490 true 1494 true 691 true 1800 true 692 true 1801 true 536 true 1645 true 1495 true 1496 true 693 true 1802 true 703 true 704 true 694 true 1803 true 705 true 706 true 707 true 708 true 709 true 710 true 711 true 712 true 713 true 714 true 720 true 715 true 1497 true 716 true 717 true 537 true 1646 true 718 true 695 true 696 true 1503 true 697 true 698 true 699 true 700 true 719 true 1504 true 1505 true 701 true 1507 true 1508 true 1510 true 631 true 1740 true 632 true 1741 true 1514 true 1515 true 1516 true 1517 true 1518 true 1520 true 1521 true 1522 true 1523 true 1524 true 1526 true 1528 true 1529 true 1530 true 1531 true 1532 true 1534 true 1535 true 1537 true 1538 true 1539 true 1540 true 1541 true 1542 true 1543 true 1545 true 1546 true 1547 true 1548 true 1549 true 1550 true 1551 true 1552 true 1553 true 1554 true 1555 true 1556 true 1557 true 1558 true 1559 true 450 true 603 true 1712 true 604 true 1713 true 598 true 1707 true 1560 true 451 true 1561 true 452 true 1562 true 453 true 1563 true 454 true 1564 true 455 true 1565 true 456 true 1566 true 457 true 1567 true 458 true 1568 true 459 true 1569 true 460 true 1572 true 1573 true 1576 true 1583 true 1584 true 475 true 1585 true 476 true 1586 true 477 true 1587 true 478 true 1588 true 479 true 1589 true 480 true 1590 true 481 true 1591 true 482 true 1592 true 483 true 1593 true 484 true 1594 true 485 true 1595 true 486 true 1596 true 487 true 1597 true 488 true 1598 true 489 true 1599 true 490 true 1600 true 491 true 1601 true 492 true 1602 true 493 true 1603 true 494 true 1604 true 495 true 1605 true 496 true 1606 true 497 true 1607 true 498 true 1608 true 499 true 1609 true 500 true 1614 true 1616 true 1617 true 1618 true 1619 true 510 true 1620 true 511 true 1621 true 512 true 1622 true 513 true 1623 true 514 true 1624 true 515 true 1626 true 1629 true 520 true 1630 true 521 true 1631 true 522 true 1632 true 523 true 1633 true 524 true 1643 true 1644 true 1647 true 1648 true 1649 true 1650 true 1652 true 543 true 1653 true 1654 true 1655 true 1656 true 1657 true 1658 true 1659 true 1660 true 1661 true 1662 true 1663 true 1664 true 1665 true 1666 true 1667 true 1668 true 1669 true 1670 true 1671 true 1672 true 1673 true 1674 true 1675 true 1676 true 1677 true 1678 true 1679 true 1680 true 1681 true 1682 true 1683 true 1684 true 1685 true 1686 true 1687 true 1689 true 1690 true 1708 true 1714 true 1715 true 1716 true 1723 true 1724 true 1725 true 1726 true 1727 true 1731 true 622 true 1732 true 1733 true 1735 true 1736 true 1737 true 1738 true 1739 true 1742 true 1743 true 1750 true 1751 true 1757 true 1759 true 1760 true 1761 true 1764 true 1765 true 1766 true 1767 true 1771 true 1772 true 1773 true 1775 true 1776 true 1777 true 1778 true 1779 true 1780 true 1781 true 1782 true 1783 true 1784 true 1785 true 1786 true 1787 true 1788 true 1789 true 1790 true 1791 true 1792 true 1793 true 1796 true 1797 true"}

dimacs_file = "../../inputs/" + data["Instance"]
solution_str = data["Solution"]

if check_satisfaction(dimacs_file, solution_str):
    print("Solution satisfies the DIMACS problem.")
else:
    print("Solution does NOT satisfy the DIMACS problem.")
