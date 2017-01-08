/*
 * Tests for PIO data reading and writing routines.
 *
 * Ed Hartnett
 */
#include <pio.h>
#include <pio_tests.h>

/* The number of tasks this test should run on. */
#define TARGET_NTASKS 4

/* The minimum number of tasks this test should run on. */
#define MIN_NTASKS 4

/* The name of this test. */
#define TEST_NAME "test_pioc_putget"

/* Number of processors that will do IO. */
#define NUM_IO_PROCS 1

/* Number of computational components to create. */
#define COMPONENT_COUNT 1

/* The number of dimensions in the example data. In this test, we
 * are using three-dimensional data. */
#define NDIM 3

/* The length of our sample data along each dimension. */
#define X_DIM_LEN 40
#define Y_DIM_LEN 40

/* The number of timesteps of data to write. */
#define NUM_TIMESTEPS 1

/* The name of the variable in the netCDF output files. */
#define VAR_NAME "foo"

/* The name of the attribute in the netCDF output files. */
#define ATT_NAME "bar"

/* The meaning of life, the universe, and everything. */
#define START_DATA_VAL 42

/* Values for some netcdf-4 settings. */
#define VAR_CACHE_SIZE (1024 * 1024)
#define VAR_CACHE_NELEMS 10
#define VAR_CACHE_PREEMPTION 0.5

/* Number of NetCDF classic types. */
#define NUM_CLASSIC_TYPES 6

/* Number of NetCDF-4 types. */
#define NUM_NETCDF4_TYPES 12

/* The dimension names. */
char dim_name[NDIM][PIO_MAX_NAME + 1] = {"timestep", "x", "y"};

/* Length of the dimensions in the sample data. */
int dim_len[NDIM] = {NC_UNLIMITED, X_DIM_LEN, Y_DIM_LEN};

/* Length of chunksizes to use in netCDF-4 files. */
PIO_Offset chunksize[NDIM] = {2, X_DIM_LEN/2, Y_DIM_LEN/2};

/* Some sample data values to write. */
char char_data = 2;
signed char byte_data = -42;
short short_data = -300;
int int_data = -10000;
float float_data = -42.42;
double double_data = -420000000000.5;
unsigned char ubyte_data = 43;
unsigned short ushort_data = 666;
unsigned int uint_data = 666666;
long long int64_data = -99999999999;
unsigned long long uint64_data = 99999999999;

char char_array[X_DIM_LEN][Y_DIM_LEN];
signed char byte_array[X_DIM_LEN][Y_DIM_LEN];
short short_array[X_DIM_LEN][Y_DIM_LEN];
int int_array[X_DIM_LEN][Y_DIM_LEN];
float float_array[X_DIM_LEN][Y_DIM_LEN];
double double_array[X_DIM_LEN][Y_DIM_LEN];
unsigned char ubyte_array[X_DIM_LEN][Y_DIM_LEN];
unsigned short ushort_array[X_DIM_LEN][Y_DIM_LEN];
unsigned int uint_array[X_DIM_LEN][Y_DIM_LEN];
long long int64_array[X_DIM_LEN][Y_DIM_LEN];
unsigned long long uint64_array[X_DIM_LEN][Y_DIM_LEN];

#define DIM_NAME "dim"
#define NDIM1 1
#define DIM_LEN 4

/* Fill up the data arrays with some values. */
void init_arrays()
{
    for (int x = 0; x < X_DIM_LEN; x++)
        for (int y = 0; y < Y_DIM_LEN; y++)
        {
            char_array[x][y] = char_data;
            byte_array[x][y] = byte_data;
            short_array[x][y] = short_data;
            int_array[x][y] = int_data;
            float_array[x][y] = float_data;
            double_array[x][y] = double_data;
            ubyte_array[x][y] = ubyte_data;
            ushort_array[x][y] = ushort_data;
            uint_array[x][y] = uint_data;
            int64_array[x][y] = int64_data;
            uint64_array[x][y] = uint64_data;
        }
}

/* Use the var1 functions to write some data to an open test file. */
int putget_write_var1(int ncid, int *varid, PIO_Offset *index, int flavor)
{
    int ret;

    /* if ((ret = PIOc_put_var1_text(ncid, varid[2], index, &char_data))) */
    /*     ERR(ret); */

    if ((ret = PIOc_put_var1_schar(ncid, varid[0], index, (signed char *)byte_array)))
        return ret;

    if ((ret = PIOc_put_var1_short(ncid, varid[2], index, &short_data)))
        return ret;

    if ((ret = PIOc_put_var1_int(ncid, varid[3], index, &int_data)))
        return ret;

    if ((ret = PIOc_put_var1_float(ncid, varid[4], index, &float_data)))
        return ret;

    if ((ret = PIOc_put_var1_double(ncid, varid[5], index, &double_data)))
        return ret;

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_put_var1_uchar(ncid, varid[6], index, &ubyte_data)))
            return ret;
        if ((ret = PIOc_put_var1_ushort(ncid, varid[7], index, &ushort_data)))
            return ret;
        if ((ret = PIOc_put_var1_uint(ncid, varid[8], index, &uint_data)))
            return ret;
        if ((ret = PIOc_put_var1_longlong(ncid, varid[9], index, &int64_data)))
            return ret;
        if ((ret = PIOc_put_var1_ulonglong(ncid, varid[10], index, &uint64_data)))
            return ret;
    }

    return 0;
}

/* Use the var1 functions to write some data to an open test file. */
int putget_write_var(int ncid, int *varid, int flavor)
{
    int ret;

    /* if ((ret = PIOc_put_var_text(ncid, varid[2], &char_array))) */
    /*     ERR(ret); */

    if ((ret = PIOc_put_var_schar(ncid, varid[0], (signed char *)byte_array)))
        return ret;

    if ((ret = PIOc_put_var_short(ncid, varid[2], (short *)short_array)))
        return ret;

    if ((ret = PIOc_put_var_int(ncid, varid[3], (int *)int_array)))
        return ret;

    if ((ret = PIOc_put_var_float(ncid, varid[4], (float *)float_array)))
        return ret;

    if ((ret = PIOc_put_var_double(ncid, varid[5], (double *)double_array)))
        return ret;

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_put_var_uchar(ncid, varid[6], (unsigned char *)ubyte_array)))
            return ret;
        if ((ret = PIOc_put_var_ushort(ncid, varid[7], (unsigned short *)ushort_array)))
            return ret;
        if ((ret = PIOc_put_var_uint(ncid, varid[8], (unsigned int *)uint_array)))
            return ret;
        if ((ret = PIOc_put_var_longlong(ncid, varid[9], (long long *)int64_array)))
            return ret;
        if ((ret = PIOc_put_var_ulonglong(ncid, varid[10], (unsigned long long *)uint64_array)))
            return ret;
    }

    return 0;
}

/* Use the vara functions to write some data to an open test file. */
int putget_write_vara(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                      int flavor)
{
    int ret;

    /* if ((ret = PIOc_put_vara_text(ncid, varid[2], start, count, char_array))) */
    /*     ERR(ret); */

    if ((ret = PIOc_put_vara_schar(ncid, varid[0], start, count, (signed char *)byte_array)))
        return ret;

    if ((ret = PIOc_put_vara_short(ncid, varid[2], start, count, (short *)short_array)))
        return ret;

    if ((ret = PIOc_put_vara_int(ncid, varid[3], start, count, (int *)int_array)))
        return ret;

    if ((ret = PIOc_put_vara_float(ncid, varid[4], start, count, (float *)float_array)))
        return ret;

    if ((ret = PIOc_put_vara_double(ncid, varid[5], start, count, (double *)double_array)))
        return ret;

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_put_vara_uchar(ncid, varid[6], start, count, (unsigned char *)ubyte_array)))
            return ret;
        if ((ret = PIOc_put_vara_ushort(ncid, varid[7], start, count, (unsigned short *)ushort_array)))
            return ret;
        if ((ret = PIOc_put_vara_uint(ncid, varid[8], start, count, (unsigned int *)uint_array)))
            return ret;
        if ((ret = PIOc_put_vara_longlong(ncid, varid[9], start, count, (long long *)int64_array)))
            return ret;
        if ((ret = PIOc_put_vara_ulonglong(ncid, varid[10], start, count, (unsigned long long *)uint64_array)))
            return ret;
    }

    return 0;
}

/* Use the vars functions to write some data to an open test file. */
int putget_write_vars(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                      PIO_Offset *stride, int flavor)
{
    int ret;

    /* if ((ret = PIOc_put_vara_text(ncid, varid[2], start, count, char_array))) */
    /*     ERR(ret); */

    if ((ret = PIOc_put_vars_schar(ncid, varid[0], start, count, stride, (signed char *)byte_array)))
        return ret;

    if ((ret = PIOc_put_vars_short(ncid, varid[2], start, count, stride, (short *)short_array)))
        return ret;

    if ((ret = PIOc_put_vars_int(ncid, varid[3], start, count, stride, (int *)int_array)))
        return ret;

    if ((ret = PIOc_put_vars_float(ncid, varid[4], start, count, stride, (float *)float_array)))
        return ret;

    if ((ret = PIOc_put_vars_double(ncid, varid[5], start, count, stride, (double *)double_array)))
        return ret;

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_put_vars_uchar(ncid, varid[6], start, count, stride, (unsigned char *)ubyte_array)))
            return ret;
        if ((ret = PIOc_put_vars_ushort(ncid, varid[7], start, count, stride, (unsigned short *)ushort_array)))
            return ret;
        if ((ret = PIOc_put_vars_uint(ncid, varid[8], start, count, stride, (unsigned int *)uint_array)))
            return ret;
        if ((ret = PIOc_put_vars_longlong(ncid, varid[9], start, count, stride, (long long *)int64_array)))
            return ret;
        if ((ret = PIOc_put_vars_ulonglong(ncid, varid[10], start, count, stride, (unsigned long long *)uint64_array)))
            return ret;
    }

    return 0;
}

/* Use the var1 functions to read some data from an open test file. */
int putget_read_var1(int ncid, int *varid, PIO_Offset *index, int flavor)
{
    signed char byte_data_in;
    short short_data_in;
    unsigned char ubyte_data_in;
    int int_data_in;
    float float_data_in;
    double double_data_in;
    unsigned short ushort_data_in;
    unsigned int uint_data_in;
    long long int64_data_in;
    unsigned long long uint64_data_in;
    int ret;

    if ((ret = PIOc_get_var1_schar(ncid, varid[0], index, &byte_data_in)))
        return ret;
    if (byte_data_in != byte_data)
        return ERR_WRONG;

    if ((ret = PIOc_get_var1_short(ncid, varid[2], index, &short_data_in)))
        return ret;
    if (short_data_in != short_data)
        return ERR_WRONG;

    if ((ret = PIOc_get_var1_int(ncid, varid[3], index, &int_data_in)))
        return ret;
    if (int_data_in != int_data)
        return ERR_WRONG;

    if ((ret = PIOc_get_var1_float(ncid, varid[4], index, &float_data_in)))
        return ret;
    if (float_data_in != float_data)
        return ERR_WRONG;

    if ((ret = PIOc_get_var1_double(ncid, varid[5], index, &double_data_in)))
        return ret;
    if (double_data_in != double_data)
        return ERR_WRONG;

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_get_var1_uchar(ncid, varid[6], index, &ubyte_data_in)))
            return ret;
        if (ubyte_data_in != ubyte_data)
            return ERR_WRONG;
        if ((ret = PIOc_get_var1_ushort(ncid, varid[7], index, &ushort_data_in)))
            return ret;
        if (ushort_data_in != ushort_data)
            return ERR_WRONG;
        if ((ret = PIOc_get_var1_uint(ncid, varid[8], index, &uint_data_in)))
            return ret;
        if (uint_data_in != uint_data)
            return ERR_WRONG;
        if ((ret = PIOc_get_var1_longlong(ncid, varid[9], index, &int64_data_in)))
            return ret;
        if (int64_data_in != int64_data)
            return ERR_WRONG;
        if ((ret = PIOc_get_var1_ulonglong(ncid, varid[10], index, &uint64_data_in)))
            return ret;
        if (uint64_data_in != uint64_data)
            return ERR_WRONG;
    }

    return 0;
}

/* Use the var functions to read some data from an open test file.
 *
 * @param ncid the ncid of the test file to read.
 * @param varid an array of varids in the file.
 * @param unlim non-zero if unlimited dimension is in use.
 * @param flavor the PIO IO type of the test file.
 * @returns 0 for success, error code otherwise.
*/
int putget_read_var(int ncid, int *varid, int unlim, int flavor)
{
    signed char byte_array_in[X_DIM_LEN][Y_DIM_LEN];
    short short_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned char ubyte_array_in[X_DIM_LEN][Y_DIM_LEN];
    int int_array_in[X_DIM_LEN][Y_DIM_LEN];
    float float_array_in[X_DIM_LEN][Y_DIM_LEN];
    double double_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned short ushort_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned int uint_array_in[X_DIM_LEN][Y_DIM_LEN];
    long long int64_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned long long uint64_array_in[X_DIM_LEN][Y_DIM_LEN];
    int x, y;
    int ret;

    /* When using the unlimited dimension, no data are wrtten by the
     * put_var_TYPE() functions, since the length of the unlimited
     * dimension is still 0. So for reading, just confirm that the
     * data files are empty. */
    if (unlim)
    {
        return 0;
    }

    if ((ret = PIOc_get_var_schar(ncid, varid[0], (signed char *)byte_array_in)))
        return ret;
    if ((ret = PIOc_get_var_short(ncid, varid[2], (short *)short_array_in)))
        return ret;
    if ((ret = PIOc_get_var_int(ncid, varid[3], (int *)int_array_in)))
        return ret;
    if ((ret = PIOc_get_var_float(ncid, varid[4], (float *)float_array_in)))
        return ret;
    if ((ret = PIOc_get_var_double(ncid, varid[5], (double *)double_array_in)))
        return ret;
    for (x = 0; x < X_DIM_LEN; x++)
        for (y = 0; y < Y_DIM_LEN; y++)
        {
            if (byte_array_in[x][y] != byte_array[x][y])
                return ERR_WRONG;
            if (short_array_in[x][y] != short_array[x][y])
                return ERR_WRONG;
            if (int_array_in[x][y] != int_array[x][y])
                return ERR_WRONG;
            if (float_array_in[x][y] != float_array[x][y])
                return ERR_WRONG;
            if (double_array_in[x][y] != double_array[x][y])
                return ERR_WRONG;
        }

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_get_var_uchar(ncid, varid[6], (unsigned char *)ubyte_array_in)))
            return ret;
        if ((ret = PIOc_get_var_ushort(ncid, varid[7], (unsigned short *)ushort_array_in)))
            return ret;
        if ((ret = PIOc_get_var_uint(ncid, varid[8], (unsigned int *)uint_array_in)))
            return ret;
        if ((ret = PIOc_get_var_longlong(ncid, varid[9], (long long *)int64_array_in)))
            return ret;
        if ((ret = PIOc_get_var_ulonglong(ncid, varid[10], (unsigned long long *)uint64_array_in)))
            return ret;
        for (x = 0; x < X_DIM_LEN; x++)
            for (y = 0; y < Y_DIM_LEN; y++)
            {
                if (ubyte_array_in[x][y] != ubyte_array[x][y])
                    return ERR_WRONG;
                if (ushort_array_in[x][y] != ushort_array[x][y])
                    return ERR_WRONG;
                if (uint_array_in[x][y] != uint_array[x][y])
                    return ERR_WRONG;
                if (int64_array_in[x][y] != int64_array[x][y])
                    return ERR_WRONG;
                if (uint64_array_in[x][y] != uint64_array[x][y])
                    return ERR_WRONG;
            }
    }

    return 0;
}

/* Use the vara functions to read some data from an open test file. */
int putget_read_vara(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                     int flavor)
{
    signed char byte_array_in[X_DIM_LEN][Y_DIM_LEN];
    short short_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned char ubyte_array_in[X_DIM_LEN][Y_DIM_LEN];
    int int_array_in[X_DIM_LEN][Y_DIM_LEN];
    float float_array_in[X_DIM_LEN][Y_DIM_LEN];
    double double_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned short ushort_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned int uint_array_in[X_DIM_LEN][Y_DIM_LEN];
    long long int64_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned long long uint64_array_in[X_DIM_LEN][Y_DIM_LEN];
    int x, y;
    int ret;

    if ((ret = PIOc_get_vara_schar(ncid, varid[0], start, count, (signed char *)byte_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_short(ncid, varid[2], start, count, (short *)short_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_int(ncid, varid[3], start, count, (int *)int_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_float(ncid, varid[4], start, count, (float *)float_array_in)))
        return ret;
    if ((ret = PIOc_get_vara_double(ncid, varid[5], start, count, (double *)double_array_in)))
        return ret;

    for (x = 0; x < X_DIM_LEN; x++)
        for (y = 0; y < Y_DIM_LEN; y++)
        {
            if (byte_array_in[x][y] != byte_array[x][y])
                return ERR_WRONG;
            if (short_array_in[x][y] != short_array[x][y])
                return ERR_WRONG;
            if (int_array_in[x][y] != int_array[x][y])
                return ERR_WRONG;
            if (float_array_in[x][y] != float_array[x][y])
                return ERR_WRONG;
            if (double_array_in[x][y] != double_array[x][y])
                return ERR_WRONG;
        }

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_get_vara_uchar(ncid, varid[6], start, count, (unsigned char *)ubyte_array_in)))
            return ret;

        if ((ret = PIOc_get_vara_ushort(ncid, varid[7], start, count, (unsigned short *)ushort_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_uint(ncid, varid[8], start, count, (unsigned int *)uint_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_longlong(ncid, varid[9], start, count, (long long *)int64_array_in)))
            return ret;
        if ((ret = PIOc_get_vara_ulonglong(ncid, varid[10], start, count, (unsigned long long *)uint64_array_in)))
            return ret;
        for (x = 0; x < X_DIM_LEN; x++)
            for (y = 0; y < Y_DIM_LEN; y++)
            {
                if (ubyte_array_in[x][y] != ubyte_array[x][y])
                    return ERR_WRONG;
                if (ushort_array_in[x][y] != ushort_array[x][y])
                    return ERR_WRONG;
                if (uint_array_in[x][y] != uint_array[x][y])
                    return ERR_WRONG;
                if (int64_array_in[x][y] != int64_array[x][y])
                    return ERR_WRONG;
                if (uint64_array_in[x][y] != uint64_array[x][y])
                    return ERR_WRONG;
            }
    }

    return 0;
}

/* Use the vars functions to read some data from an open test file. */
int putget_read_vars(int ncid, int *varid, PIO_Offset *start, PIO_Offset *count,
                     PIO_Offset *stride, int flavor)
{
    signed char byte_array_in[X_DIM_LEN][Y_DIM_LEN];
    short short_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned char ubyte_array_in[X_DIM_LEN][Y_DIM_LEN];
    int int_array_in[X_DIM_LEN][Y_DIM_LEN];
    float float_array_in[X_DIM_LEN][Y_DIM_LEN];
    double double_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned short ushort_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned int uint_array_in[X_DIM_LEN][Y_DIM_LEN];
    long long int64_array_in[X_DIM_LEN][Y_DIM_LEN];
    unsigned long long uint64_array_in[X_DIM_LEN][Y_DIM_LEN];
    int x, y;
    int ret;

    if ((ret = PIOc_get_vars_schar(ncid, varid[0], start, count, stride, (signed char *)byte_array_in)))
        return ret;
    if ((ret = PIOc_get_vars_short(ncid, varid[2], start, count, stride, (short *)short_array_in)))
        return ret;
    if ((ret = PIOc_get_vars_int(ncid, varid[3], start, count, stride, (int *)int_array_in)))
        return ret;
    if ((ret = PIOc_get_vars_float(ncid, varid[4], start, count, stride, (float *)float_array_in)))
        return ret;
    if ((ret = PIOc_get_vars_double(ncid, varid[5], start, count, stride, (double *)double_array_in)))
        return ret;

    for (x = 0; x < X_DIM_LEN; x++)
        for (y = 0; y < Y_DIM_LEN; y++)
        {
            if (byte_array_in[x][y] != byte_array[x][y])
                return ERR_WRONG;
            if (short_array_in[x][y] != short_array[x][y])
                return ERR_WRONG;
            if (int_array_in[x][y] != int_array[x][y])
                return ERR_WRONG;
            if (float_array_in[x][y] != float_array[x][y])
                return ERR_WRONG;
            if (double_array_in[x][y] != double_array[x][y])
                return ERR_WRONG;
        }

    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
    {
        if ((ret = PIOc_get_vars_uchar(ncid, varid[6], start, count, stride, (unsigned char *)ubyte_array_in)))
            return ret;

        if ((ret = PIOc_get_vars_ushort(ncid, varid[7], start, count, stride, (unsigned short *)ushort_array_in)))
            return ret;
        if ((ret = PIOc_get_vars_uint(ncid, varid[8], start, count, stride, (unsigned int *)uint_array_in)))
            return ret;
        if ((ret = PIOc_get_vars_longlong(ncid, varid[9], start, count, stride, (long long *)int64_array_in)))
            return ret;
        if ((ret = PIOc_get_vars_ulonglong(ncid, varid[10], start, count, stride, (unsigned long long *)uint64_array_in)))
            return ret;
        for (x = 0; x < X_DIM_LEN; x++)
            for (y = 0; y < Y_DIM_LEN; y++)
            {
                if (ubyte_array_in[x][y] != ubyte_array[x][y])
                    return ERR_WRONG;
                if (ushort_array_in[x][y] != ushort_array[x][y])
                    return ERR_WRONG;
                if (uint_array_in[x][y] != uint_array[x][y])
                    return ERR_WRONG;
                if (int64_array_in[x][y] != int64_array[x][y])
                    return ERR_WRONG;
                if (uint64_array_in[x][y] != uint64_array[x][y])
                    return ERR_WRONG;
            }
    }

    return 0;
}

/* Create a test file for the putget tests to write data to and check
 * by reading it back. In this function we create the file, define the
 * dims and vars, and pass back the ncid.
 *
 * @param iosysid the IO system ID.
 * @param try the number of the test run, 0 for var, 1 for var1, 2 for
 * vara, 3 for vars.
 * @param unlim non-zero if unlimited dimension should be used.
 * @param flavor the PIO IO type.
 * @param dim_len array of length NDIM of the dimension lengths.
 * @param varid array of varids for the variables in the test file.
 * @param filename the name of the test file to create.
 * @param ncidp pointer that gets the ncid of the created file.
 * @returns 0 for success, error code otherwise.
 */
int create_putget_file(int iosysid, int access, int unlim, int flavor, int *dim_len,
                       int *varid, char *filename, int *ncidp)
{
    char iotype_name[PIO_MAX_NAME + 1];
    int dimids[NDIM];        /* The dimension IDs. */
    int num_vars = NUM_CLASSIC_TYPES;
    int xtype[NUM_NETCDF4_TYPES] = {PIO_BYTE, PIO_CHAR, PIO_SHORT, PIO_INT, PIO_FLOAT,
                                    PIO_DOUBLE, PIO_UBYTE, PIO_USHORT, PIO_UINT, PIO_INT64,
                                    PIO_UINT64, PIO_STRING};
    int ncid;
    int ret;

    /* Create a filename. */
    if ((ret = get_iotype_name(flavor, iotype_name)))
        return ret;
    sprintf(filename, "%s_putget_access_%d_unlim_%d_%s.nc", TEST_NAME, access, unlim,
            iotype_name);

    /* Create the netCDF output file. */
    if ((ret = PIOc_createfile(iosysid, &ncid, &flavor, filename, PIO_CLOBBER)))
        return ret;

    /* Are we using unlimited dimension? */
    if (!unlim)
        dim_len[0] = NUM_TIMESTEPS;

    /* Define netCDF dimensions and variable. */
    for (int d = 0; d < NDIM; d++)
        if ((ret = PIOc_def_dim(ncid, dim_name[d], (PIO_Offset)dim_len[d], &dimids[d])))
            return ret;

    /* For netcdf-4, there are extra types. */
    if (flavor == PIO_IOTYPE_NETCDF4C || flavor == PIO_IOTYPE_NETCDF4P)
        num_vars = NUM_NETCDF4_TYPES;

    /* Define variables. */
    for (int v = 0; v < num_vars; v++)
    {
        char var_name[PIO_MAX_NAME + 1];
        snprintf(var_name, PIO_MAX_NAME, "%s_%d", VAR_NAME, xtype[v]);
        if ((ret = PIOc_def_var(ncid, var_name, xtype[v], NDIM, dimids, &varid[v])))
            return ret;
    }

    if ((ret = PIOc_enddef(ncid)))
        return ret;

    /* Pass back the ncid. */
    *ncidp = ncid;

    return 0;
}

/* Test data read/write operations.
 *
 * This function creates a file with 3 dimensions, with a var of each
 * type. Then it uses the var/var1/vars/vars functions to write, and
 * then read data from the test file.
 *
 * @param iosysid the iosystem ID that will be used for the test.
 * @param num_flavors the number of different IO types that will be tested.
 * @param flavor an array of the valid IO types.
 * @param my_rank 0-based rank of task.
 * @returns 0 for success, error code otherwise.
 */
int test_putget(int iosysid, int num_flavors, int *flavor, int my_rank,
                MPI_Comm test_comm)
{
    int dim_len[NDIM] = {NC_UNLIMITED, X_DIM_LEN, Y_DIM_LEN};

#define NUM_ACCESS 4
    for (int unlim = 0; unlim < 2; unlim++)
        for (int access = 0; access < NUM_ACCESS; access++)
        {
            /* Use PIO to create the example file in each of the four
             * available ways. */
            for (int fmt = 0; fmt < num_flavors; fmt++)
            {
                char filename[PIO_MAX_NAME + 1]; /* Test filename. */
                int ncid;
                int varid[NUM_NETCDF4_TYPES];
                int ret;    /* Return code. */

                /* Create test file with dims and vars defined. */
                printf("%d Access %d creating test file for flavor = %d...\n",
                       my_rank, access, flavor[fmt]);
                if ((ret = create_putget_file(iosysid, access, unlim, flavor[fmt], dim_len, varid,
                                              filename, &ncid)))
                    return ret;

                /* Write some data. */
                PIO_Offset index[NDIM] = {0, 0, 0};
                PIO_Offset start[NDIM] = {0, 0, 0};
                PIO_Offset count[NDIM] = {1, X_DIM_LEN, Y_DIM_LEN};
                PIO_Offset stride[NDIM] = {1, 1, 1};

                switch (access)
                {
                case 0:
                    printf("%d Access %d writing data with var functions for flavor = %d...\n",
                           my_rank, access, flavor[fmt]);
                    /* Use the var functions to write some data. */
                    if ((ret = putget_write_var(ncid, varid, flavor[fmt])))
                        return ret;
                    break;

                case 1:
                    /* Use the var1 functions to write some data. */
                    if ((ret = putget_write_var1(ncid, varid, index, flavor[fmt])))
                        return ret;
                    break;

                case 2:
                    /* Use the vara functions to write some data. */
                    if ((ret = putget_write_vara(ncid, varid, start, count, flavor[fmt])))
                        return ret;
                    break;

                case 3:
                    /* Use the vara functions to write some data. */
                    if ((ret = putget_write_vars(ncid, varid, start, count, stride, flavor[fmt])))
                        return ret;
                    break;
                }

                /* Make sure all data are written (pnetcdf needs this). */
                if ((ret = PIOc_sync(ncid)))
                    return ret;

                switch (access)
                {
                case 0:
                    /* Use the vara functions to read some data. */
                    if ((ret = putget_read_var(ncid, varid, unlim, flavor[fmt])))
                        return ret;
                    break;

                case 1:
                    /* Use the var1 functions to read some data. */
                    if ((ret = putget_read_var1(ncid, varid, index, flavor[fmt])))
                        return ret;
                    break;

                case 2:
                    /* Use the vara functions to read some data. */
                    if ((ret = putget_read_vara(ncid, varid, start, count, flavor[fmt])))
                        return ret;
                    break;

                case 3:
                    /* Use the vara functions to read some data. */
                    if ((ret = putget_read_vars(ncid, varid, start, count, stride, flavor[fmt])))
                        return ret;
                    break;
                }

                /* Close the netCDF file. */
                if ((ret = PIOc_closefile(ncid)))
                    ERR(ret);

                /* Access to read it. */
                if ((ret = PIOc_openfile(iosysid, &ncid, &(flavor[fmt]), filename, PIO_NOWRITE)))
                    ERR(ret);

                switch (access)
                {
                case 0:
                    /* Use the vara functions to read some data. */
                    if ((ret = putget_read_var(ncid, varid, unlim, flavor[fmt])))
                        return ret;
                    break;

                case 1:
                    /* Use the var1 functions to read some data. */
                    if ((ret = putget_read_var1(ncid, varid, index, flavor[fmt])))
                        return ret;
                    break;

                case 2:
                    /* Use the vara functions to read some data. */
                    if ((ret = putget_read_vara(ncid, varid, start, count, flavor[fmt])))
                        return ret;
                    break;

                case 3:
                    /* Use the vara functions to read some data. */
                    if ((ret = putget_read_vars(ncid, varid, start, count, stride, flavor[fmt])))
                        return ret;
                    break;
                }

                /* Close the netCDF file. */
                if ((ret = PIOc_closefile(ncid)))
                    ERR(ret);

            } /* next flavor */
        } /* next access */

    return PIO_NOERR;
}

/* Run all the tests. */
int test_all(int iosysid, int num_flavors, int *flavor, int my_rank, MPI_Comm test_comm,
             int async)
{
    int ioid;
    int my_test_size;
    int ret; /* Return code. */

    if ((ret = MPI_Comm_size(test_comm, &my_test_size)))
        MPIERR(ret);

    /* Test read/write stuff. */
    printf("%d Testing putget. async = %d\n", my_rank, async);
    if ((ret = test_putget(iosysid, num_flavors, flavor, my_rank, test_comm)))
        return ret;

    return PIO_NOERR;
}

/* Run Tests for NetCDF-4 Functions. */
int main(int argc, char **argv)
{
    /* Initialize data arrays with sample data. */
    init_arrays();

    return run_test_main(argc, argv, MIN_NTASKS, TARGET_NTASKS, 3,
                         TEST_NAME, dim_len, COMPONENT_COUNT, NUM_IO_PROCS);

    return 0;
}