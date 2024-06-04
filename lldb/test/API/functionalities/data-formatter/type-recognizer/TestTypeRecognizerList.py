"""
Test lldb data formatter subsystem.
"""


from lldbsuite.test.lldbtest import *


class TypeRecognizerListTestCase(TestBase):
    def test_type_recognizer_list(self):
        """Test that the 'type recognizer list' command handles command line arguments properly"""
        self.expect(
            "type recognizer list Foo", substrs=["Category: default", "Category: system"]
        )

        self.expect("type recognizer list -w default", substrs=["system"], matching=False)
        self.expect(
            "type recognizer list -w system unsigned",
            substrs=["default", "0-9"],
            matching=False,
        )
