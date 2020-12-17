#pragma once
/*! \file cube.h
*
*  \brief cube class to handle conways game of life for day-17 challenge
*
*
*  \author Graham Riches
*
*/

#pragma once

/************************************ Includes ********************************************/
#include <vector>
#include <tuple>


/************************************ Definitions ********************************************/
/**
 * @brief 3D cube object
*/
class Cube3D
{
public:
   Cube3D( int x, int y, int z) : _x(x), _y(y), _z(z) {};
   Cube3D( int x, int y ) : _x(x), _y(y), _z(0) {};
   
   /**
    * @brief get the neighbour cubes for a cube    
    * @return vector of neighbour coordinates
    */
   std::vector<Cube3D> get_neighbours(void) const
   {
      std::vector<Cube3D> coordinates;      
      for (int x = this->_x - 1; x <= this->_x + 1; x++)
      {
         for (int y = this->_y - 1; y <= this->_y + 1; y++)
         {
            for (int z = this->_z - 1; z <= this->_z + 1; z++)
            {
               Cube3D neighbour{x, y, z};
               if (neighbour != *this)
               {
                  coordinates.push_back(neighbour);
               }
            }
         }
      }
      return coordinates;
   }

   /**
    * @brief == operator overload for convenience
   */
   friend bool operator == (const Cube3D& lhs, const Cube3D& rhs)
   {
      return std::tie(lhs._x, lhs._y, lhs._z) == std::tie(rhs._x, rhs._y, rhs._z);
   }

   /**
    * @brief != operator overload for convenience
   */
   friend bool operator != (const Cube3D& lhs, const Cube3D& rhs)
   {
      return std::tie(lhs._x, lhs._y, lhs._z) != std::tie(rhs._x, rhs._y, rhs._z);
   }

   /**
    * @brief < operator overload for convenience
   */
   friend bool operator < (const Cube3D& lhs, const Cube3D& rhs)
   {
      return std::tie(lhs._x, lhs._y, lhs._z) < std::tie(rhs._x, rhs._y, rhs._z);
   }

   /**
    * @brief > operator overload for convenience
   */
   friend bool operator > (const Cube3D& lhs, const Cube3D& rhs)
   {
      return std::tie(lhs._x, lhs._y, lhs._z) > std::tie(rhs._x, rhs._y, rhs._z);
   }

private:
   int _x;
   int _y;
   int _z;

};


/**
 * @brief 4D cube object
*/
class Cube4D
{
public:
   Cube4D( int x, int y, int z, int w ) : _x( x ), _y( y ), _z( z ), _w( w ) {};
   Cube4D( int x, int y ) : _x( x ), _y( y ), _z( 0 ), _w( 0) {};

   /**
    * @brief get the neighbour cubes for a cube
    * @return vector of neighbour coordinates
    */
   std::vector<Cube4D> get_neighbours( void ) const
   {
      std::vector<Cube4D> coordinates;
      for (int x = this->_x - 1; x <= this->_x + 1; x++)
      {
         for (int y = this->_y - 1; y <= this->_y + 1; y++)
         {
            for (int z = this->_z - 1; z <= this->_z + 1; z++)
            {
               for (int w = this->_w - 1; w <= this->_w + 1; w++)
               {
                  Cube4D neighbour{ x, y, z, w };
                  if (neighbour != *this)
                  {
                     coordinates.push_back( neighbour );
                  }
               }               
            }
         }
      }
      return coordinates;
   }

   /**
    * @brief == operator overload for convenience
   */
   friend bool operator == ( const Cube4D& lhs, const Cube4D& rhs )
   {
      return std::tie( lhs._x, lhs._y, lhs._z, lhs._w ) == std::tie( rhs._x, rhs._y, rhs._z, rhs._w );
   }

   /**
    * @brief != operator overload for convenience
   */
   friend bool operator != ( const Cube4D& lhs, const Cube4D& rhs )
   {
      return std::tie( lhs._x, lhs._y, lhs._z, lhs._w ) != std::tie( rhs._x, rhs._y, rhs._z, rhs._w );
   }

   /**
    * @brief < operator overload for convenience
   */
   friend bool operator < ( const Cube4D& lhs, const Cube4D& rhs )
   {
      return std::tie( lhs._x, lhs._y, lhs._z, lhs._w ) < std::tie( rhs._x, rhs._y, rhs._z, rhs._w );
   }

   /**
    * @brief > operator overload for convenience
   */
   friend bool operator > ( const Cube4D& lhs, const Cube4D& rhs )
   {
      return std::tie( lhs._x, lhs._y, lhs._z, lhs._w ) > std::tie( rhs._x, rhs._y, rhs._z, rhs._w );
   }

private:
   int _x;
   int _y;
   int _z;
   int _w;

};