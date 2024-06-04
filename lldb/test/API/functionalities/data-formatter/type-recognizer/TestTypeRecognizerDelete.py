"""
Test lldb data formatter subsystem.
"""


from lldbsuite.test.lldbtest import *


class TypeRecognizerListTestCase(TestBase):
    def test_type_recognizer_delete(self):
        """Test that the 'type recognizer delete' command works properly"""

        self.runCmd("type recognizer add Foo -F testDefault")
        self.runCmd("type recognizer add Foo -F testFooCat -w FooCat")
        self.runCmd("type recognizer add Bar -F testDefault")
        self.runCmd("type recognizer add Bar -F testBarCat -w BarCat")
        self.runCmd("type recognizer add Bar -F testBarCatSecond -w BarCatSecond")

        self.runCmd("type recognizer delete Foo")
        self.expect(
            "type recognizer list",
            substrs=[
                "Bar:  Python function testDefault",
                "Bar:  Python function testBarCat",
                "Bar:  Python function testBarCatSecond",
                "Foo:  Python function testFooCat",
            ],
        )
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function testDefault",
            ],
            matching=False,
        )

        self.runCmd("type recognizer delete Bar -w BarCatSecond")
        self.expect(
            "type recognizer list",
            substrs=[
                "Bar:  Python function testDefault",
                "Bar:  Python function testBarCat",
                "Foo:  Python function testFooCat",
            ],
        )
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function testDefault",
                "Bar:  Python function testBarCatSecond",
            ],
            matching=False,
        )

        self.runCmd("type recognizer delete Bar -a")
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function testFooCat",
            ],
        )
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function testDefault",
                "Bar:  Python function testDefault",
                "Bar:  Python function testBarCat",
                "Bar:  Python function testBarCatSecond",
            ],
            matching=False,
        )
