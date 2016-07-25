/* Support for *xattr interfaces on GNU/Hurd.
   Copyright (C) 2006-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <hurd.h>
#include <hurd/xattr.h>
#include <string.h>
#include <sys/mman.h>

error_t
_hurd_xattr_get (io_t port, const char *name, void *value, size_t *size)
{
  char *buf = value;
  size_t bufsz = value ? *size : 0;
  error_t err = 0;

  err = __file_get_xattr (port, name, &buf, &bufsz);

  if (err)
    return err;
  if (value != NULL && *size < bufsz)
    {
      if (buf != value)
        munmap (buf, bufsz);
      return -ERANGE;
    }
  if (buf != value && bufsz > 0)
    {
      if (value != NULL)
        memcpy (value, buf, bufsz);
      munmap (buf, bufsz);
    }

  *size = bufsz;
  return 0;
}

error_t
_hurd_xattr_set (io_t port, const char *name, const void *value, size_t size,
		 int flags)
{
  return __file_set_xattr (port, name, value, size, flags);
}

error_t
_hurd_xattr_remove (io_t port, const char *name)
{
  return _hurd_xattr_set (port, name, NULL, 0, 0);
}

error_t
_hurd_xattr_list (io_t port, void *buffer, size_t *size)
{
  char *buf = buffer;
  size_t bufsz = buffer ? *size : 0;
  error_t err = 0;

  err = __file_list_xattr (port, &buf, *bufsz);

  if (err)
    return err;
  if (buffer != NULL && *size < bufsz)
    {
      if (buf != buffer)
	munmap (buf, bufsz);
      return -ERANGE;
    }
  if (buffer != buf && bufsz > 0)
    {
      if (buffer != NULL)
	memcpy (buffer, buf, bufsz);
      munmap (buf, bufsz);
    }

  *size = bufsz
  return 0;
}
