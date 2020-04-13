using System;
using System.Text;
using System.Net.Http;
using System.Threading.Tasks;
using System.Net;
using Newtonsoft.Json;
using static System.Net.Http.HttpMethod;

namespace Rest
{
    /// <summary>
    /// Used to package the status code and the response body of an HttpResponse into a single object
    /// </summary>
    public struct Response
    {
        public HttpStatusCode Status { get; set; }

        public dynamic Data { get; set; }
    }

    /// <summary>
    /// A REST client to use in testing
    /// </summary>
    public class RestClient : HttpClient
    {
        /// <summary>
        /// Create a RestClient for the specified base URL
        /// </summary>
        public RestClient(string baseURL)
        {
            BaseAddress = new Uri(baseURL);
            DefaultRequestHeaders.Add("Accept", "application/json");
        }

        /// <summary>
        /// Returns a Task that produces the Response to an Http request
        /// </summary>
        public async Task<Response> DoMethodAsync(string method, string url, dynamic data = null)
        {
            StringContent content = new StringContent(JsonConvert.SerializeObject(data), Encoding.UTF8, "application/json");
            HttpResponseMessage response;

            switch (method)
            {
                case "GET":
                    response = await GetAsync(url);
                    break;

                case "POST":
                    response = await PostAsync(url, content);
                    break;

                case "PUT":
                    response = await PutAsync(url, content);
                    break;

                case "DELETE":
                    response = await DeleteAsync(url);
                    break;

                default:
                    throw new Exception("Invalid HTTP method");
            }

            if (response.IsSuccessStatusCode)
            {
                string result = await response.Content.ReadAsStringAsync();
                return new Response { Status = response.StatusCode, Data = JsonConvert.DeserializeObject(result) };
            }
            else
            {
                return new Response { Status = response.StatusCode };
            }
        }
    }
}
