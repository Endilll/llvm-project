"""
Test lldb data formatter subsystem.
"""


from lldbsuite.test.lldbtest import *


class TypeRecognizerListTestCase(TestBase):
    def test_type_recognizer_add(self):
        """Test that the 'type recognizer add' command works properly"""

        self.runCmd("type recognizer add -F testFunc Aoo")
        self.runCmd("type recognizer add -F testFunc Boo -C false")
        self.runCmd("type recognizer add -F testFunc Coo -p")
        self.runCmd("type recognizer add -F testFunc Doo -r")
        self.runCmd("type recognizer add -F testFunc Eoo -p -r -C false")
        self.runCmd("type recognizer add -F testFunc -x '^Foo$'")

        self.runCmd("type recognizer add -F testFunc Goo -w NewCat")

        self.expect(
            "type recognizer list -w default",
            substrs=[
                "Aoo:  Python function testFunc",
                "Boo:  (not cascading) Python function testFunc",
                "Coo:  (skip pointers) Python function testFunc",
                "Doo:  (skip references) Python function testFunc",
                "Eoo:  (not cascading) (skip pointers) (skip references) Python function testFunc",
                "^Foo$:  Python function testFunc",
            ],
        )

        self.expect(
            "type recognizer list -w NewCat",
            substrs=[
                "Goo:  Python function testFunc",
            ],
        )
