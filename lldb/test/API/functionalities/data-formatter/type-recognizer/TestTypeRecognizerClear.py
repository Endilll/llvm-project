"""
Test lldb data formatter subsystem.
"""


from lldbsuite.test.lldbtest import *


class TypeRecognizerListTestCase(TestBase):
    def test_type_recognizer_clear(self):
        """Test that the 'type recognizer clear' command works properly"""

        self.runCmd("type recognizer add Foo -F test")
        self.runCmd("type recognizer add Bar -F test -w BarCat")
        self.runCmd("type recognizer add Zoo -F test -w ZooCat")

        self.runCmd("type recognizer clear ZooCat")
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function test",
                "Bar:  Python function test",
            ],
        )
        self.expect(
            "type recognizer list",
            substrs=[
                "Zoo:  Python function test",
            ],
            matching=False,
        )

        self.runCmd("type recognizer clear")
        self.expect(
            "type recognizer list",
            substrs=[
                "Bar:  Python function test",
            ],
        )
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function test",
                "Zoo:  Python function test",
            ],
            matching=False,
        )

        self.runCmd("type recognizer clear -a")
        self.expect(
            "type recognizer list",
            substrs=[
                "Foo:  Python function test",
                "Bar:  Python function test",
                "Zoo:  Python function test",
            ],
            matching=False,
        )
