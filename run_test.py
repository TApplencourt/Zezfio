#!/usr/bin/env python

import unittest
from zezfio.database import LegacyFolderHieracy

class TestSetLegacyFolder(unittest.TestCase):

    def test_init_db(self):
        db_path = "../test/min.ezfio/"
        LegacyFolderHieracy(db_path)

    def test_init_db_wrong_path(self):
        db_path = "../test/min"
        with self.assertRaises(IOError):
            LegacyFolderHieracy(db_path)

class TestIntScalar(unittest.TestCase):

    path_config ="../test/zezfio.json"
    db_path = "../test/min.ezfio/"
    db = LegacyFolderHieracy(db_path)

    def test_read_scalar(self):
        i = self.__class__.db.read_scalar("dimension","det_num","int")
        self.assertEqual(i,10000000)

    def test_read_scalar_wrong_path(self):
        with self.assertRaises(IOError):
            i = self.__class__.db.read_scalar("dimension","wtf","int")

    def test_read_scalar_wrong_type(self):
        with self.assertRaises(TypeError):
            i = self.__class__.db.read_scalar("dimension","det_num","wrong")


class TestIntArray(unittest.TestCase):

    path_config ="../test/zezfio.json"
    db_path = "../test/min.ezfio/"
    db = LegacyFolderHieracy(db_path)

    def test_read_array(self):
        i = self.__class__.db.read_array("aarray","det_num","int",10000000)
        self.assertEqual(i[0],7267)
        self.assertEqual(i[-1],19334)
        self.assertIsInstance(i[0],int)

    def test_read_array_wrong_path(self):
        with self.assertRaises(IOError):
            i = self.__class__.db.read_array("aarray","wtf","int",10000000)
    
    def test_read_array_wrong_size(self):
        with self.assertRaises(RuntimeError):
            i = self.__class__.db.read_array("aarray","det_num","int",100)

    def test_read_array_wrong_size(self):
        with self.assertRaises(RuntimeError):
            i = self.__class__.db.read_array("aarray","det_num","int",10000000000000)

if __name__ == '__main__':
    unittest.main()
