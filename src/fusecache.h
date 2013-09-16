//------------------------------------------------------------------------------
// Copyright (c) 2012-2013 by European Organization for Nuclear Research (CERN)
// Author: Justin Salmon <jsalmon@cern.ch>
//------------------------------------------------------------------------------
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with This program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef __CACHEFS_HPP__
#define __CACHEFS_HPP__

#include <llfusexx.h>
#include <iostream>
#include <cstring>

namespace fusecache
{
  //----------------------------------------------------------------------------
  //! The main layer between FUSE and the user filesystem implementation. This
  //! is a write-through cache which caches both file data and metadata.
  //!
  //! We use the curiously recurring template pattern to subclass llfusexx::fs
  //! hence forcing us to implement all the low-level fuse functions.
  //!
  //! We define several pure virtual functions that the user subclass must
  //! implement.
  //----------------------------------------------------------------------------
  template <typename T>
  class fs : public llfusexx::fs< fs<T> >
  {
    public:
      //------------------------------------------------------------------------
      //! States that the networked filesystem can be in
      //------------------------------------------------------------------------
      enum fusecache_status_t
      {
        ONLINE,         //!< the network server is online
        OFFLINE,        //!< the network server is offline
        SYNCHRONIZING   //!< the network server is online, but synchronization
                        //!< is still in progress
      };

      //------------------------------------------------------------------------
      //! Constructor
      //------------------------------------------------------------------------
      fs() {};

      //------------------------------------------------------------------------
      //! Destructor
      //------------------------------------------------------------------------
      virtual ~fs() {};

      //------------------------------------------------------------------------
      //! @return status of the network server
      //------------------------------------------------------------------------
      virtual fusecache_status_t status() = 0;

      std::string cached;

    public:
      //------------------------------------------------------------------------
      //! Initialize filesystem
      //------------------------------------------------------------------------
      static void init( void *userdata, struct fuse_conn_info *conn )
      {
        std::cout << "init()" << std::endl;
        T::init( userdata, conn );
      }

      //------------------------------------------------------------------------
      //! Clean up filesystem
      //------------------------------------------------------------------------
      static void destroy( void *userdata )
      {
        std::cout << "destroy()" << std::endl;
        T::destroy( userdata );
      }

      //------------------------------------------------------------------------
      //! Get file attributes
      //------------------------------------------------------------------------
      static void getattr( fuse_req_t             req,
                           fuse_ino_t             ino,
                           struct fuse_file_info *fi )
      {
        std::cout << "getattr()" << std::endl;
        T::getattr( req, ino, fi );
      }

      //------------------------------------------------------------------------
      //! Change attributes of a file
      //------------------------------------------------------------------------
      static void setattr( fuse_req_t             req,
                           fuse_ino_t             ino,
                           struct stat           *attr,
                           int                    to_set,
                           struct fuse_file_info *fi )
      {
        std::cout << "setattr()" << std::endl;
        T::setattr( req, ino, attr, to_set, fi );
      }

      //------------------------------------------------------------------------
      //! Lookup an entry
      //------------------------------------------------------------------------
      static void lookup( fuse_req_t  req,
                          fuse_ino_t  parent,
                          const char *name )
      {
        std::cout << "lookup()" << std::endl;
        T::lookup( req, parent, name );
      }

      //------------------------------------------------------------------------
      //! Read the entries from a directory
      //------------------------------------------------------------------------
      static void readdir( fuse_req_t             req,
                           fuse_ino_t             ino,
                           size_t                 size,
                           off_t                  off,
                           struct fuse_file_info *fi )
      {
        std::cout << "readdir()" << std::endl;
        T::readdir( req, ino, size, off, fi );
      }

      //------------------------------------------------------------------------
      //! Drop directory view
      //------------------------------------------------------------------------
      static void releasedir( fuse_req_t             req,
                              fuse_ino_t             ino,
                              struct fuse_file_info *fi )
      {
        std::cout << "releasedir()" << std::endl;
        T::releasedir( req, ino, fi );
      }

      //------------------------------------------------------------------------
      //! Return statistics about the filesystem
      //------------------------------------------------------------------------
      static void statfs( fuse_req_t req, fuse_ino_t ino )
      {
        std::cout << "statfs()" << std::endl;
        T::statfs( req, ino );
      }

      //------------------------------------------------------------------------
      //! Make a special (device) file, FIFO, or socket
      //------------------------------------------------------------------------
      static void mknod( fuse_req_t  req,
                         fuse_ino_t  parent,
                         const char *name,
                         mode_t      mode,
                         dev_t       rdev )
      {
        std::cout << "mknod()" << std::endl;
        T::mknod( req, parent, name, mode, rdev );
      }

      //------------------------------------------------------------------------
      //! Create a directory with a given name
      //------------------------------------------------------------------------
      static void mkdir( fuse_req_t  req,
                         fuse_ino_t  parent,
                         const char *name,
                         mode_t      mode )
      {
        std::cout << "mkdir()" << std::endl;
        T::mkdir( req, parent, name, mode );
      }

      //------------------------------------------------------------------------
      //! Remove a file
      //------------------------------------------------------------------------
      static void unlink( fuse_req_t req, fuse_ino_t parent, const char *name )
      {
        std::cout << "unlink()" << std::endl;
        T::unlink( req, parent, name );
      }

      //------------------------------------------------------------------------
      //! Remove the given directory
      //------------------------------------------------------------------------
      static void rmdir( fuse_req_t req, fuse_ino_t parent, const char *name )
      {
        std::cout << "rmdir()" << std::endl;
        T::rmdir( req, parent, name );
      }

      //------------------------------------------------------------------------
      //! Rename the file, directory, or other object
      //------------------------------------------------------------------------
      static void rename( fuse_req_t  req,
                          fuse_ino_t  parent,
                          const char *name,
                          fuse_ino_t  newparent,
                          const char *newname )
      {
        std::cout << "rename()" << std::endl;
        T::rename( req, parent, name, newparent, newname );
      }

      //------------------------------------------------------------------------
      //! Check file access permissions
      //------------------------------------------------------------------------
      static void access( fuse_req_t req, fuse_ino_t ino, int mask )
      {
        std::cout << "access()" << std::endl;
        T::access( req, ino, mask );
      }

      //------------------------------------------------------------------------
      //! Open a file
      //------------------------------------------------------------------------
      static void open( fuse_req_t             req,
                        fuse_ino_t             ino,
                        struct fuse_file_info *fi )
      {
        std::cout << "open()" << std::endl;
        T::open( req, ino, fi );
      }

      //------------------------------------------------------------------------
      //! Open a directory
      //------------------------------------------------------------------------
      static void opendir( fuse_req_t             req,
                           fuse_ino_t             ino,
                           struct fuse_file_info *fi )
      {
        std::cout << "opendir()" << std::endl;
        T::opendir( req, ino, fi );
      }

      //------------------------------------------------------------------------
      //! Read from file.
      //------------------------------------------------------------------------
      static void read( fuse_req_t             req,
                        fuse_ino_t             ino,
                        size_t                 size,
                        off_t                  off,
                        struct fuse_file_info *fi )
      {
        std::cout << "read()" << std::endl;

        if (T::self->status() == ONLINE)
        {
          std::cout << "client online" << std::endl;

          if (T::self->cached.size() != 0)
          {
            std::cout << "reading from cache" << std::endl;
            fuse_reply_buf(req, T::self->cached.c_str(), T::self->cached.size());
            return;
          }
          else
          {
            //T::read ( req, ino, size, off, fi );
            std::cout << "reading from client" << std::endl;
            int ret = T::self->read( ino, size, off, T::self->cached );
            fuse_reply_buf( req, T::self->cached.c_str(), T::self->cached.size() );
            return;
          }
        }
        else
        {
          std::cout << "client offline" << std::endl;
          fuse_reply_buf(req, T::self->cached.c_str(), T::self->cached.size());
          return;
        }
      }

      virtual int read( fuse_ino_t ino, size_t size, off_t off, std::string &buf ) = 0;

      //------------------------------------------------------------------------
      //! Write function
      //------------------------------------------------------------------------
      static void write( fuse_req_t             req,
                         fuse_ino_t             ino,
                         const char            *buf,
                         size_t                 size,
                         off_t                  off,
                         struct fuse_file_info *fi )
      {
        std::cout << "write()" << std::endl;
        T::write( req, ino, buf, size, off, fi );
      }

      //------------------------------------------------------------------------
      //! Release an open file
      //------------------------------------------------------------------------
      static void release( fuse_req_t             req,
                           fuse_ino_t             ino,
                           struct fuse_file_info *fi )
      {
        std::cout << "release()" << std::endl;
        T::release( req, ino, fi );
      }

      //------------------------------------------------------------------------
      //! Flush any dirty information about the file to disk
      //------------------------------------------------------------------------
      static void fsync( fuse_req_t             req,
                         fuse_ino_t             ino,
                         int                    datasync,
                         struct fuse_file_info *fi )
      {
        std::cout << "fsync()" << std::endl;
        T::fsync( req, ino, datasync, fi );
      }

      //------------------------------------------------------------------------
      //! Forget inode <-> path mapping
      //------------------------------------------------------------------------
      static void forget( fuse_req_t req, fuse_ino_t ino, unsigned long nlookup )
      {
        std::cout << "forget()" << std::endl;
        T::forget( req, ino, nlookup );
      }

      //------------------------------------------------------------------------
      //! Flush method
      //------------------------------------------------------------------------
      static void flush( fuse_req_t             req,
                         fuse_ino_t             ino,
                         struct fuse_file_info *fi )
      {
        std::cout << "flush()" << std::endl;
        T::flush( req, ino, fi );
      }

      //------------------------------------------------------------------------
      //! Get an extended attribute
      //------------------------------------------------------------------------
#ifdef __APPLE__
      static void getxattr( fuse_req_t  req,
                            fuse_ino_t  ino,
                            const char *name,
                            size_t      size,
                            uint32_t    position )
#else
      static void getxattr( fuse_req_t  req,
                            fuse_ino_t  ino,
                            const char *name,
                            size_t      size )
#endif
      {
        std::cout << "getxattr()" << std::endl;
        T::getxattr( req, ino, name, size );
      }

    //--------------------------------------------------------------------------
    //! Set extended attribute
    //--------------------------------------------------------------------------
#ifdef __APPLE__
      static void setxattr( fuse_req_t  req,
                            fuse_ino_t  ino,
                            const char *name,
                            const char *value,
                            size_t      size,
                            int         flags,
                            uint32_t    position )
#else
      static void setxattr( fuse_req_t  req,
                            fuse_ino_t  ino,
                            const char *name,
                            const char *value,
                            size_t      size,
                            int         flags )
#endif
      {
        std::cout << "setxattr()" << std::endl;
      }

      //------------------------------------------------------------------------
      //! List extended attributes
      //------------------------------------------------------------------------
      static void listxattr( fuse_req_t req, fuse_ino_t ino, size_t size )
      {
        std::cout << "listxattr()" << std::endl;
        T::listxattr( req, ino, size );
      }

      //------------------------------------------------------------------------
      //! Remove extended attribute
      //------------------------------------------------------------------------
      static void removexattr( fuse_req_t  req,
                               fuse_ino_t  ino,
                               const char *xattr_name )
      {
        std::cout << "removexattr()" << std::endl;
        T::removexattr( req, ino, xattr_name );
      }

      //------------------------------------------------------------------------
      //! Run the thing
      //------------------------------------------------------------------------
      int daemonize( int argc, char* argv[], T *self, void *userdata )
      {
        return llfusexx::fs<fs>::daemonize( argc, argv, self, userdata );
      }
  };
}

#endif /* __CACHEFS_HPP__ */
