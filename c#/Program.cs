using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;

using SSEClient.DB;

namespace SSEConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            //connect to SSE-DB
            //SSE-DB works on port 2912
            Connection connection = new Connection("192.168.50.189");
            connection.GetConnection();

            //Database Creation
            String databaseName = "employee";
            //Encrypt the data
            byte[] enc_databaseName = PerformEncryption(databaseName);

            //create database
            int status = connection.CreateDB(enc_databaseName);

            if (status == 1)
            {
                Console.WriteLine("Database created successfully");
            }
            else
            {
                Console.WriteLine("Database creation failed");
            }


            //Table Creation
            String tableName = "emp_details";
            String column_name_1 = "name";
            String column_name_2 = "age";

            //Encrypt the data
            byte[] enc_tableName = PerformEncryption(tableName);
            byte[] enc_column_name_1 = PerformEncryption(column_name_1);
            byte[] enc_column_name_2 = PerformEncryption(column_name_2);

            //create table
            status = connection.CreateTable(enc_databaseName, enc_tableName, enc_column_name_1, enc_column_name_2);

            if (status == 1)
            {
                Console.WriteLine("Table created successfully");
            }
            else
            {
                Console.WriteLine("Table creation failed");
            }

            //insert values in table
            String emp1_name = "tom";
            String emp1_age = "20";

            String emp2_name = "alice";
            String emp2_age = "25";

            String emp3_name = "eve";
            String emp3_age = "30";

            //Encrypt the data
            byte[] enc_emp1_name = PerformEncryption(emp1_name);
            byte[] enc_emp1_age = PerformEncryption(emp1_age);

            byte[] enc_emp2_name = PerformEncryption(emp2_name);
            byte[] enc_emp2_age = PerformEncryption(emp2_age);

            byte[] enc_emp3_name = PerformEncryption(emp3_name);
            byte[] enc_emp3_age = PerformEncryption(emp3_age);

            //insert
            status = connection.Insert(enc_databaseName, enc_tableName, enc_column_name_1, enc_emp1_name, enc_column_name_2, enc_emp1_age);
            if (status == 1)
            {
                Console.WriteLine("Insertion successful");
            }
            else
            {
                Console.WriteLine("Insertion failed");
            }

            status = connection.Insert(enc_databaseName, enc_tableName, enc_column_name_1, enc_emp2_name, enc_column_name_2, enc_emp2_age);
            if (status == 1)
            {
                Console.WriteLine("Insertion successful");
            }
            else
            {
                Console.WriteLine("Insertion failed");
            }

            status = connection.Insert(enc_databaseName, enc_tableName, enc_column_name_1, enc_emp3_name, enc_column_name_2, enc_emp3_age);
            if (status == 1)
            {
                Console.WriteLine("Insertion successful");
            }
            else
            {
                Console.WriteLine("Insertion failed");
            }

            //fetch values from SSE-DB
            Connection.ResultSet rs = connection.Select(enc_databaseName, enc_tableName, enc_column_name_1, enc_column_name_2);
            while (rs.hasNext())
            {
                byte[] name = rs.get(enc_column_name_1);
                byte[] age = rs.get(enc_column_name_2);

                if (name == null)
                {
                    Console.WriteLine("val is null");
                }
                else
                {
                    Console.WriteLine("Name: " + PerformDecryption(name));
                    Console.WriteLine("Age: " + PerformDecryption(age));
                }
            }

            //Close connection to SSE-DB
            connection.Close();

            Console.WriteLine("Exit .....");

            Console.ReadLine();
        }


        //This section contains crypto operations for the demo

        //needless to say "KEY SHOULD NOT BE HARDCODED"
        static byte[] key = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
        static byte[] iv = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

        
        public static byte[] PerformEncryption(String plainText)
        {
            byte[] encrypted;
            using (Aes aesAlg = Aes.Create())
            {
                aesAlg.Key = key;
                aesAlg.IV = iv;

                aesAlg.Mode = CipherMode.CBC;

                var encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

                // Create the streams used for encryption. 
                using (var msEncrypt = new MemoryStream())
                {
                    using (var csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write))
                    {
                        using (var swEncrypt = new StreamWriter(csEncrypt))
                        {
                            
                            swEncrypt.Write(plainText);
                        }
                        encrypted = msEncrypt.ToArray();
                    }
                }


                return encrypted;
            }
        }

        public static String PerformDecryption(byte[] cipherText)
        {
            string plaintext = null;

            using (Aes aesAlg = Aes.Create())
            {
                aesAlg.Key = key;
                aesAlg.IV = iv;

                aesAlg.Mode = CipherMode.CBC;

                // Create a decrytor to perform the stream transform.
                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                // Create the streams used for decryption. 
                using (var msDecrypt = new MemoryStream(cipherText))
                {
                    using (var csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read))
                    {
                        using (var srDecrypt = new StreamReader(csDecrypt))
                        {

                            
                            plaintext = srDecrypt.ReadToEnd();
                        }
                    }
                }

            }

            return plaintext;

        }

    }

    

}
