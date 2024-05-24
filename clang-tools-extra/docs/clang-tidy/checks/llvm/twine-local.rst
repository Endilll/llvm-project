.. title:: clang-tidy - llvm-twine-local

llvm-twine-local
================


Looks for local ``llvm::Twine`` variables which are prone to use after frees and
should be generally avoided.

.. code-block:: c++

  static llvm::Twine Moo = llvm::Twine("bark") + "bah";

  // becomes

  static std::string Moo = (llvm::Twine("bark") + "bah").str();
